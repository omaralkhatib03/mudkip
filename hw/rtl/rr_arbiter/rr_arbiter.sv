`timescale 1ns/1ps

module rr_arbiter #(
  parameter NUM_INPUTS    = 4,
  parameter NUM_OUTPUTS   = 4,
  parameter DATA_WIDTH    = 8,
  parameter IN_FIFO_DEPTH = 8
) (
  input   wire                      clk,
  input   wire                      rst_n,

  input   wire [NUM_INPUTS-1:0]     req,
  input   wire [DATA_WIDTH-1:0]     in[NUM_INPUTS-1:0],

  output logic [NUM_OUTPUTS-1:0]    valid,
  output logic [DATA_WIDTH-1:0]     dout[NUM_OUTPUTS-1:0],
  input  wire  [NUM_OUTPUTS-1:0]    ready
);

    function automatic logic [NUM_INPUTS-1:0] getInputTurn(
        logic [NUM_INPUTS-1:0] base,
        logic [NUM_INPUTS-1:0] req_calc
    );
        logic [NUM_INPUTS-1:0] grant;
        logic [2*NUM_INPUTS-1:0] double_req;
        logic [2*NUM_INPUTS-1:0] double_grant;

        // https://stackoverflow.com/questions/55015328/understanding-a-simple-round-robin-arbiter-verilog-code
        double_req ={req_calc,req_calc};
        double_grant = double_req & ~(double_req-(NUM_INPUTS * 2)'(base));
        grant = double_grant[NUM_INPUTS-1:0] | double_grant[2*NUM_INPUTS-1:NUM_INPUTS];

        return grant;
    endfunction


endmodule
