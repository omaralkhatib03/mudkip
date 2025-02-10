`timescale 1ns/1ps

// Hardcoded as shit, needs docs, CBA 

// The idea is simple, I have one PS interface which gives me data to send to
// some location in the design. I have a limited number of interfaces, and I'm
// not typically concerned with latency here. I need to PS to configure the PL
// before running but not much. Solution ? Create a core/node block which has
// childs. Each child has a unique ID which is defined somewhere. The
// PS simply feeds into this module. This module directs the thingy to the
// correct child.

module ps_core # (
  parameter NODES = 3,
  parameter DATA_WIDTH = 32
) (
  input clk,
  input rst_n,

  ps_if.slave ps_top, // TODO: Might Need to adjust, Idk whos replying to me unless SW keeps track of this

  ps_if.master  ps_nodes[NODES-1:0]
);
 
  localparam mm2s_x = 0, mm2s_y = 1, s2mm_out = 2;
  
  logic valid_mm2x;
  logic valid_mm2y;
  logic valid_s2mm_out;

  ps_if_copier ps_copier_mm2s_x_I (clk, valid_mm2x, ps_top, ps_nodes[mm2s_x]);
  ps_if_copier ps_copier_mm2s_y_I (clk, valid_mm2y, ps_top, ps_nodes[mm2s_y]);
  ps_if_copier ps_copier_s2mm_I   (clk, valid_s2mm_out, ps_top, ps_nodes[s2mm_out]);

  always_ff @(posedge clk) 
  begin 
    if (!rst_n)
    begin
      valid_mm2x      <= 0;
      valid_mm2y      <= 0;
      valid_s2mm_out  <= 0;
    end
    else 
    begin
      case (ps_top.node_addr)
      mm2s_x:
      begin
        valid_mm2x      <= 1;
      end
      mm2s_y:
      begin
        valid_mm2x      <= 1;
      end
      s2mm_out:
      begin
       valid_s2mm_out   <= 1; 
      end
      default:
      begin
        valid_mm2x      <= 0;
        valid_mm2y      <= 0;
        valid_s2mm_out  <= 0;
      end
      endcase 
    end
  end

  logic [DATA_WIDTH + 1:0] din[NODES-1:0];
  logic [NODES-1:0] rvalids;

  generate
    genvar i;
    for (i = 0; i < NODES; i = i + 1) begin : gen_ps_nodes
      assign din[i] = {ps_nodes[i].rdata, ps_nodes[i].wresp, ps_nodes[i].wready};
      assign rvalids[i] = ps_nodes[i].rvalid;
    end
  endgenerate

  rr_arbiter #(
    .NUM_INPUTS(NODES),              
    .DATA_WIDTH(DATA_WIDTH),
    .IN_FIFO_DEPTH(8)
  ) ps_core_arbiter_I (
    .clk(clk),                        
    .rst_n(rst_n),                      
    .req(rvalids),       
    .in(din), 
    .dout({ps_top.rdata, ps_top.wresp, ps_top.wready}),
    .valid(ps_top.rvalid),
    .ready(ps_top.rready) 
  );  

endmodule
