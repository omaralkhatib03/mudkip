`timescale 1ns/1ps

module fmul #(
    parameter PARALLELISM = 3,
    parameter DELAY = 3,
    parameter BIT_SIZE = 32,
    parameter FRACTION_SIZE_OR = 23,
    parameter EXPONENT_SIZE_OR = 8,
    parameter CUSTOM_FORMAT = 0
) (
  input wire clk,
  input wire rst_n,
  
  input wire [BIT_SIZE-1:0]   a[PARALLELISM-1:0],
  input wire                  valid_a,
  output wire                 ready_a,
  input wire                  tlast_a,
  
  input wire [BIT_SIZE-1:0]   b[PARALLELISM-1:0],
  input wire                  valid_b,
  output wire                 ready_b,
  input wire                  tlast_b,

  input wire                  sub,

  output wire [BIT_SIZE-1:0]  out, // If sub computes a - b
  output wire                 valid,
  input wire                  ready,
  output wire                 tlast
);

  logic [BIT_SIZE-1:0] res[PARALLELISM-1:0];

  logic empty_a;
  logic empty_b;

  logic full_a;
  logic full_b;

  logic shift_out;

  logic fifo_valid_a;
  logic fifo_valid_b;

  logic [BIT_SIZE-1:0] fifo_a[PARALLELISM-1:0];
  logic fifo_last_a;

  logic [BIT_SIZE-1:0] fifo_b[PARALLELISM-1:0];
  logic fifo_last_b;
  
  assign shift_out = !(empty_b && empty_a);

  /* verilator lint_off PINMISSING */
  basic_sync_fifo #(
    .DATA_WIDTH   (1),
    .DEPTH        (4),
    .READ_LATENCY (0)
  ) a_fifo_I (
    .clk          (clk),
    .rst_n        (rst_n),

    .din          (tlast_a),
    .shift_in     (valid_a),

    .shift_out    (shift_out),
    .valid        (fifo_valid_a),
    .dout         (fifo_last_a),
    .empty        (empty_a),
    .full         (full_a)
  );

  basic_sync_fifo #(
    .DATA_WIDTH   (1),
    .DEPTH        (4),
    .READ_LATENCY (0)
  ) b_fifo_I (
    .clk          (clk),
    .rst_n        (rst_n),

    .din          (tlast_b),
    .shift_in     (valid_b),

    .shift_out    (shift_out),
    .valid        (fifo_valid_b),
    .dout         (fifo_last_b),
    .empty        (empty_b),
    .full         (full_b)
  );
  /* verilator lint_on PINMISSING */

  genvar i;
  generate
    
    for (i = 0; i < PARALLELISM; i++)
    begin : fifos_module
      /* verilator lint_off PINMISSING */
      basic_sync_fifo #(
        .DATA_WIDTH   (BIT_SIZE),
        .DEPTH        (4),
        .READ_LATENCY (0)
      ) a_fifo_I (
        .clk          (clk),
        .rst_n        (rst_n),

        .din          (a[i]),
        .shift_in     (valid_a),

        .shift_out    (shift_out),
        // .valid        (),
        .dout         (fifo_a[i])
        // .empty        (),
        // .full         ()
      );

      basic_sync_fifo #(
        .DATA_WIDTH   (BIT_SIZE),
        .DEPTH        (4),
        .READ_LATENCY (0)
      ) b_fifo_I (
        .clk          (clk),
        .rst_n        (rst_n),

        .din          (b[i]),
        .shift_in     (valid_b),

        .shift_out    (shift_out),
        // .valid        (),
        .dout         (fifo_b[i])
        // .empty        ()
        // .full         ()
      );
      /* verilator lint_on PINMISSING */
      
      `ifdef  VERILATOR
      /* verilator lint_off PINMISSING */
        fp_mult #(
        .BIT_SIZE(32),
        .EXPONENT(8),
        .MANITSSA(23) 
        ) fp_mult_I (
	      .a_operand(fifo_a[i]),
	      .b_operand(fifo_b[i]),
	      .result(res[i])
        );      
      /* verilator lint_on PINMISSING */
      `endif
      `ifndef VERILATOR // I.e HW
      // TODO: ADD IP INST, Manually for now  
      `endif


    end
  endgenerate
  
  generate
    `ifdef VERILATOR 
    if (DELAY == 0)
    `endif
    `ifndef VERILATOR // I.e HW
    if (1)
    `endif
    begin : combinational_fadd
      assign out    = res;
      assign valid  = fifo_valid_b && fifo_valid_a; 
      assign tlast  = fifo_last_a && fifo_last_b;
    end
    else 
    begin : delay_fadd

        delay #(
          .DATAWIDTH(BIT_SIZE + 2),
          .DELAY(DELAY)
        ) delay_I (
          .clk(clk),
          .in({res, fifo_last_a && fifo_last_b, fifo_valid_b && fifo_valid_a}),
          .out({out, tlast, valid})
        );

    end
  endgenerate

  assign ready_a = full_a | ready;
  assign ready_b = full_b | ready;

endmodule
