`timescale 1ns/1ps
// Hardcoded as shit, needs docs, CBA 

// The idea is simple, I have one PS interface which gives me data to send to
// some location in the design. I have a limited number of interfaces, and I'm
// not typically concerned with latency here. I need to PS to configure the PL
// before running but not much. Solution ? Create a core/node block which has
// childs. Each child has a unique ID which is defined somewhere. The
// PS simply feeds into this module. This module directs the thingy to the
// correct child.

module reg_core # (
  parameter NODES = 3,
  parameter DATA_WIDTH = 32
) (
    input clk,
    input rst_n,

    // TODO: Add Reg Top if
    
    
    reg_if.master  reg_nodes[NODES-1:0]
);

    localparam mm2s_x = 0, mm2s_y = 1, s2mm_out = 2;

endmodule
