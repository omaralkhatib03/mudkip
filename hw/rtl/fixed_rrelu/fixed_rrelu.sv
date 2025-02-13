`timescale 1ns / 1ps

module fixed_rrelu #(
    /* verilator lint_off UNUSEDPARAM */
    parameter DATA_IN_0_PRECISION_0 /*verilator public*/ = 8,
    parameter DATA_IN_0_PRECISION_1 /*verilator public*/ = 4,

    parameter DATA_IN_0_TENSOR_SIZE_DIM_0 = 8,
    parameter DATA_IN_0_TENSOR_SIZE_DIM_1 = 4,

    parameter DATA_IN_0_PARALLELISM_DIM_0 /*verilator public*/ = 1,
    parameter DATA_IN_0_PARALLELISM_DIM_1 /*verilator public*/ = 1,

    parameter DATA_OUT_0_PRECISION_0 /*verilator public*/ = 8,
    parameter DATA_OUT_0_PRECISION_1 /*verilator public*/ = 4,

    parameter DATA_OUT_0_TENSOR_SIZE_DIM_0 = 0,
    parameter DATA_OUT_0_TENSOR_SIZE_DIM_1 = 0,

    parameter DATA_OUT_0_PARALLELISM_DIM_0  /*verilator public*/= 1,
    parameter DATA_OUT_0_PARALLELISM_DIM_1 /*verilator public*/ = 1,

    parameter INPLACE     = 0,
    parameter LOWER       = 2, 
    parameter UPPER       = 8,
    parameter LFSR_POLY   = 32'h04c11db7  // Default is Ethernet FCS Polynomial
    /* verilator lint_on UNUSEDPARAM */
) (
    /* verilator lint_off UNUSEDSIGNAL */
    input rst_n,
    input clk,

    input logic [DATA_IN_0_PRECISION_0-1:0] data_in_0[DATA_IN_0_PARALLELISM_DIM_0*DATA_IN_0_PARALLELISM_DIM_1-1:0],
    output logic [DATA_OUT_0_PRECISION_0-1:0] data_out_0[DATA_OUT_0_PARALLELISM_DIM_0*DATA_OUT_0_PARALLELISM_DIM_1-1:0],
    /* verilator lint_on UNUSEDSIGNAL */

    input  logic data_in_0_valid,
    output logic data_in_0_ready,

    output logic data_out_0_valid,
    input  logic data_out_0_ready
);

  `define ASSERT_POWER_OF_TWO(param) \
      initial begin \
          if ((((param - 1) & (param)) != 0) || (param == 0)) begin \
              $error("Assertion failed: Parameters %s (%0d) is not power of two", `"param`", param); \
          end \
      end

  `ASSERT_POWER_OF_TWO(LOWER)
  `ASSERT_POWER_OF_TWO(UPPER)

  initial 
  begin
    if (LOWER >= UPPER)
    begin
        $error("Assertion failed: Lower is not Lower that Upper. Lower (%0d), Upper (%0d)", LOWER, UPPER); 
    end
  end

  localparam LFSR_WIDTH = DATA_IN_0_PRECISION_0;
  localparam LFSR_MASK  = LFSR_WIDTH'({1'b1, UPPER'((UPPER - 1))});

  for (
      genvar i = 0; i < DATA_IN_0_PARALLELISM_DIM_0 * DATA_IN_0_PARALLELISM_DIM_1; i++
  ) begin : RRelu 

    logic [LFSR_WIDTH-1:0] lfsr_state = '1; // Start at '1
    logic [LFSR_WIDTH-1:0] next_state;

    /* verilator lint_off UNUSEDSIGNAL */
    logic [2*DATA_IN_0_PRECISION_0-1:0] dout; 
    /* verilator lint_on UNUSEDSIGNAL */

    logic [LFSR_WIDTH-1:0] multiplier = (next_state | LOWER) & LFSR_MASK;

    always @(posedge clk) 
    begin
        lfsr_state <= next_state;
    end
    
    // You can find this module in mase/verilog-ethernet, or at https://github.com/alexforencich/verilog-lfsr/blob/master/rtl/lfsr.v
    lfsr #(
        .LFSR_WIDTH         (LFSR_WIDTH),
        .LFSR_POLY          (LFSR_POLY),
        .STYLE              ("AUTO")
    ) lfsr_I (

        // verilator lint_off PINCONNECTEMPTY 
        .data_in(),
        .data_out(),
        // verilator lint_on PINCONNECTEMPTY 
        
        .state_in(lfsr_state),

        .state_out(next_state)
    );
      
    fixed_mult #(
        .IN_A_WIDTH(DATA_IN_0_PRECISION_0),
        .IN_B_WIDTH(DATA_IN_0_PRECISION_0)
    ) fixed_mult_inst (
        .data_a (data_in_0[i]),
        .data_b (multiplier), 
        .product(dout)
    );       
    
    always_ff @(posedge clk) 
    begin : RReluOutput 
      if ($signed(data_in_0[i]) < 0)
      begin
        data_out_0[i] <= DATA_OUT_0_PRECISION_0'(dout); 
      end
      else 
      begin
        data_out_0[i] <= data_in_0[i]; 
      end
    end
  end
  
  always_ff @(posedge clk) 
  begin 
    if (!rst_n)
    begin
      data_out_0_valid <= '0;
    end
    else
    begin
      data_out_0_valid <= data_in_0_valid;
    end
  end

  assign data_in_0_ready  = data_out_0_ready;

endmodule

