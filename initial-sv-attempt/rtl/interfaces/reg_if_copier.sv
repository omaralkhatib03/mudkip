`timescale 1ns/1ps

module reg_if_copier #(
  parameter DELAY = 1
) (
  input wire clk,
  input wire hold_valid,
  reg_if.slave in_slave,
  reg_if.master in_master
);
 
generate
if (DELAY == 0)
begin : delay_0

end
else 
begin : delay_one

end
endgenerate

endmodule
