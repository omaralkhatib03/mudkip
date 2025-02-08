`timescale 1 ns/1 ps

module rw_2d_ram 
#(
  parameter DEPTH = 32,
  parameter WIDTH = 32,
  localparam ADDR_LEN = $clog2(DEPTH)
) (
  input wire clk, 
  input wire rst_n,

  // read intf 
  input wire valid_addr_ps,
  input wire [ADDR_LEN-1:0] r_addr,

  output logic [WIDTH-1:0] r_data,
  output logic valid_data,
  
  // write intf
  input wire valid_w,
  input wire [ADDR_LEN-1:0] w_addr,
  input wire [WIDTH-1:0] w_data,

  output logic ready_pl 
);

  logic [WIDTH-1:0] mem_r [DEPTH-1:0];
  logic [WIDTH-1:0] mem_b [DEPTH-1:0];

  always_comb
  begin
    mem_b = mem_r;
    
    if (valid_w)
    begin
      mem_b[w_addr] = w_data;  
    end
  end

  always_ff @(posedge clk)
  begin
    if (!rst_n)
    begin
      for (int i = 0; i < DEPTH; i++)
      begin
        mem_r[i] <= 0;
      end
    end
    else
    begin
      mem_r <= mem_b;
    end
  end

  always_ff @(posedge clk) 
  begin 
    r_data     <= mem_r[r_addr];
    valid_data <= valid_addr_ps;
  end

  assign ready_pl = 0;

endmodule
