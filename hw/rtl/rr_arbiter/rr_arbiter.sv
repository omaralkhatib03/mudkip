`timescale 1ns/1ps

module rr_arbiter #(
  parameter NUM_INPUTS = 4,              
  parameter DATA_WIDTH = 8,
  parameter IN_FIFO_DEPTH = 8
) (
  input  wire clk,                        
  input  wire rst_n,                      

  input  wire [NUM_INPUTS-1:0] req,       
  input  wire [DATA_WIDTH-1:0] in[NUM_INPUTS-1:0], 

  output logic  [DATA_WIDTH-1:0] dout,
  output logic  valid,
  input wire    ready 
);  

function automatic logic [NUM_INPUTS-1:0] getInputTurn (
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

  logic [NUM_INPUTS-1:0] shift_outs;
  logic [NUM_INPUTS-1:0] valid_outs;
  logic [NUM_INPUTS-1:0] fifos_empty;

  logic [NUM_INPUTS-1:0] base;
  logic [NUM_INPUTS-1:0] base_b;

  logic out_ready; 
  logic [NUM_INPUTS-1:0] grant;
  logic [$clog2(NUM_INPUTS)-1:0] grant_idx;
  logic [NUM_INPUTS-1:0] shift_outs_b;

  logic [DATA_WIDTH-1:0] fifo_outs[NUM_INPUTS-1:0];

  logic have_req;
  
  assign have_req = |fifos_empty;

  genvar i;
  
  oh_to_binary #(
    .INPUT_WIDTH (NUM_INPUTS) 
  ) oh_to_binary_I (grant, grant_idx);

  generate
    for (i = 0; i < NUM_INPUTS; i++)
    begin : in_fifos_gen
      basic_sync_fifo #(
        .DATA_WIDTH(DATA_WIDTH),
        .DEPTH(IN_FIFO_DEPTH),
        .READ_LATENCY(0)
      ) ps_fifo_I (
        .clk        (clk),
        .rst_n      (rst_n),

        .din        (in[i]),
        .shift_in   (req[i]),

        .shift_out  (shift_outs[i]),
        .dout       (fifo_outs[i]),
        .valid      (valid_outs[i]),

        .full       (),
        .empty      (fifos_empty[i]),
        .overflow   (),
        .underflow  () 

      );
    end
  endgenerate
 
 always_comb
  begin
    shift_outs_b  = '0;
    base_b        = base;
    valid         = '0;

    if (ready && have_req)
    begin
      shift_outs_b[grant_idx] = '1;
      base_b                  = (1 << ((grant + 1) % NUM_INPUTS));
      dout                    = fifo_outs[grant_idx];
      valid                   = '1;
    end

  end

 always_ff @(posedge clk) 
  begin 
    if (!rst_n)
    begin
      base        <= '1; 
      shift_outs  <= '0;
      grant       <= '0; 
    end
    else
    begin
      base        <= base_b;
      shift_outs  <= shift_outs_b;
      grant       <= getInputTurn(base, valid_outs); 
    end
  end

endmodule
