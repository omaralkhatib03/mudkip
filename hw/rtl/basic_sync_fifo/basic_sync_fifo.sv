`timescale 1 ns/1 ps


module basic_sync_fifo #(
  parameter DATA_WIDTH        = 32,  /* DATA_WIDTH verilator public */
  parameter DEPTH             = 16,  /* DEPTH verilator public  */
  parameter READ_LATENCY      = 0    /* READ_LATENCY verilator public  */
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
  
  logic [DATA_WIDTH-1:0] fifo_out;
  
  logic [PTR_WIDTH-1:0] rd_ptr_r;
  logic [PTR_WIDTH-1:0] rd_ptr_b;

  logic [PTR_WIDTH-1:0] wr_ptr_r;
  logic [PTR_WIDTH-1:0] wr_ptr_b;

  logic valid_i;
  
  assign underflow  = shift_out && empty;   
  assign overflow   = shift_in && full;

  always_comb
  begin
    rd_ptr_b = rd_ptr_r;
    wr_ptr_b = wr_ptr_r;
    fifo_out = mem[rd_ptr_r];
    
    if (shift_in && !full)
      wr_ptr_b = wr_ptr_r + 1;

    if (shift_out && !empty)
      rd_ptr_b = rd_ptr_r + 1;  

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
      valid_i       <= shift_out;
    end
  end
  
  generate
    if (READ_LATENCY == 0)
    begin : fifo_0_latency
      assign dout   = shift_in ? din : fifo_out;
      assign valid  = shift_out; 
      assign empty  = rd_ptr_r == wr_ptr_r && !shift_in;
      assign full   = 32'(wr_ptr_r) == DEPTH-1 && !shift_in;
    end
    else 
    begin : fifo_1_latency

      always_ff @(posedge clk) 
      begin 
        dout   <= fifo_out; 
      end

      assign valid  = valid_i;
      assign full   = 32'(wr_ptr_r) == DEPTH-1;
      assign empty  = rd_ptr_r == wr_ptr_r;
    end
  endgenerate

endmodule;

