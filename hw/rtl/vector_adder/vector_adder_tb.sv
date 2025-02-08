`timescale 1 ps/1 ps

module vector_adder_tb 
  #(
    parameter   NUMBERS       = 1,
    parameter   NUMBER_WIDTH  = 32,
    parameter   VECTOR_LENGTH = 32,
    localparam  DATA_WIDTH    = NUMBER_WIDTH * NUMBERS,
    localparam  INDEX_WIDTH   = $clog2(VECTOR_LENGTH) * NUMBERS,
    localparam  NMASK_WIDTH   = $clog2(NUMBERS)
  ) (

    input wire                          clk,
    input wire                          res_n,

    output wire                         x_ready,
    input wire                          x_valid,
    input wire [DATA_WIDTH-1:0]         x_data,
    input wire [INDEX_WIDTH-1:0]        x_index,
    input wire                          x_last,
    input wire [NMASK_WIDTH-1:0]        x_keep,

    input wire                          ready,
    output wire                         valid,
    output wire [DATA_WIDTH-1:0]        data,
    output wire [INDEX_WIDTH-1:0]       index,
    output wire                         last,
    output wire [NMASK_WIDTH-1:0]       keep

  );
  
  
  vector_if x();
  vector_if y();
  vector_if sum();

  always_comb
  begin
    x.ready   = 0;
    x.valid   = x_valid;
    x.data    = x_data;
    x.index   = x_index; 
    x.last    = x_last;
    x.keep    = x_keep;

    y.ready   = x.ready;
    y.valid   = x.valid;
    y.data    = x.data;
    y.index   = x.index; 
    y.last    = x.last;
    y.keep    = x.keep;
  end

  vector_adder #(
  ) dut_I (
    .clk     (clk),
    .res_n   (res_n),
    .x       (x),     
    .y       (y),
    .sum     (sum)
  );
  
  assign sum.ready  = ready;
  assign valid      = sum.valid;
  assign data       = sum.data;
  assign index      = sum.index;
  assign last       = sum.last;
  assign keep       = sum.keep;

endmodule
