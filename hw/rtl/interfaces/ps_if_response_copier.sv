`timescale 1ns/1ps

// TODO: Add multiple delay cycles, dont need that rn
module ps_if_response_copier #(
  parameter DELAY = 1
) (
  input wire clk,
  input wire hold_valid,
  ps_if.slave in_slave,
  ps_if.master in_master
);
 
generate
if (DELAY == 0)
begin : delay_0
    assign in_master.waddr      = in_slave.waddr;  // Address to write to
    assign in_master.wdata      = in_slave.wdata;  // Data to write
    assign in_master.wvalid     = hold_valid && in_slave.wvalid; // Data valid

    assign in_slave.wready      = in_master.wready; // Ready to Write 
    assign in_slave.wresp       = in_master.wresp;  // done writing (optional)

    assign in_master.raddr      = in_slave.raddr;    // Address to read from 

    assign in_master.arvalid    = hold_valid && in_slave.arvalid;  // Address valid
    assign in_slave.rdata       = in_master.rdata;    // data read

    assign in_slave.rvalid      = in_master.rvalid;   // valid data out
    assign in_master.rready     = in_slave.rready;   // hold on cant accept a read req 

end
else 
begin : delay_one
  always_ff @(posedge clk) 
  begin 
    in_master.waddr      <= in_slave.waddr;  // Address to write to
    in_master.wdata      <= in_slave.wdata;  // Data to write
    in_master.wvalid     <= hold_valid && in_slave.wvalid; // Data valid

    in_slave.wready      <= in_master.wready; // Ready to Write 
    in_slave.wresp       <= in_master.wresp;  // done writing (optional)

    in_master.raddr      <= in_slave.raddr;    // Address to read from 

    in_master.arvalid    <= hold_valid && in_slave.arvalid;  // Address valid
    in_slave.rdata       <= in_master.rdata;    // data read

    in_slave.rvalid      <= in_master.rvalid;   // valid data out
    in_master.rready     <= in_slave.rready;   // hold on cant accept a read req 

  end
end
endgenerate

endmodule
