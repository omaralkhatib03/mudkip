`timescale 1 ns/1 ps


module basic_sync_fifo #(
  parameter DATA_WIDTH       /* verilator public */  = 32,
  parameter DEPTH            /* verilator public */  = 4,
  parameter READ_LATENCY     /* verilator public */  = 1
) (
  input wire                    clk,
  input wire                    rst_n,

  input wire [DATA_WIDTH-1:0]   din,
  input wire                    shift_in,
  input wire                    shift_out,
  output logic [DATA_WIDTH-1:0] dout,
  output logic                  valid,

  output logic                  full,
  output logic                  empty,
  output logic                  overflow,
  output logic                  underflow
);

  localparam PTR_WIDTH = $clog2(DEPTH);

  logic [DATA_WIDTH-1:0] mem [DEPTH-1:0];

  logic [PTR_WIDTH-1:0] rd_ptr_r;
  logic [PTR_WIDTH-1:0] rd_ptr_b;

  logic [PTR_WIDTH-1:0] wr_ptr_r;
  logic [PTR_WIDTH-1:0] wr_ptr_b;

  // verilator lint_off UNUSED
  logic                     valid_i;
  logic [DATA_WIDTH-1:0]    fifo_out;
  // verilator lint_on UNUSED

  assign underflow  = shift_out && empty;
  assign overflow   = shift_in && full;
    
  always_comb
  begin
    rd_ptr_b = rd_ptr_r;
    wr_ptr_b = wr_ptr_r;
    
    if (READ_LATENCY)
    begin
        if (shift_in && !full)
          wr_ptr_b = wr_ptr_r + 1;
    end
    else 
    begin
        if (shift_in && !full && !shift_out)
          wr_ptr_b = wr_ptr_r + 1;
    end
    
    if (READ_LATENCY)
    begin
        if (shift_out && !empty)
          rd_ptr_b = rd_ptr_r + 1;
    end
    else 
    begin
        if (!shift_out && shift_in && !empty)
          rd_ptr_b = rd_ptr_r + 1;
    end

  end

  always_ff @(posedge clk)
  begin
    if (!rst_n)
    begin
      wr_ptr_r      <= '0;
      rd_ptr_r      <= '0;
      valid_i       <= '0;
    end
    else
    begin
      rd_ptr_r      <= rd_ptr_b;
      wr_ptr_r      <= wr_ptr_b;
      mem[wr_ptr_r] <= din;
    end
  end

  generate
    if (READ_LATENCY == 0)
    begin : fifo_0_latency
      assign dout     = shift_in && empty ? din : mem[rd_ptr_r];
      assign valid    = shift_in || !empty;
      assign empty    = rd_ptr_r == wr_ptr_r;
      assign full     = (wr_ptr_r + 1'b1 == rd_ptr_r);
    end
    else
    begin : fifo_1_latency
      assign dout       = mem[rd_ptr_r];
      assign valid      = !empty;
      assign full       = (wr_ptr_r + 1'b1 == rd_ptr_r);
      assign empty      = (rd_ptr_r == wr_ptr_r);
    end
  endgenerate

endmodule;

