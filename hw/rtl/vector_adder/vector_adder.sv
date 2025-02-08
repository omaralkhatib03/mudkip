`timescale 1 ns/1 ps

/*
* Adds two vectors together
*/

//`define ASSERT_VALID_ARTIH_VECT(vector_if.slave x_v, vector_if.slave y_v) \
//     assert (y_v.DATAWIDTH == x_v.DATAWIDTH) \
//     assert (y_v.FLOAT == x_v.FLOAT) \
//     assert (y_v.MANTISSA == x_v.MANTISSA) \
//     assert (y_v.VECTOR_LENGTH == x_v.VECTOR_LENGTH) \
//     assert (y_v.FLOAT == x_v.FLOAT) \
//     assert (y_v.MANTISSA == x_v.MANTISSA) \
//     assert (y_v.EXPONENT == x_v.EXPONENT) \
//     assert (y_v.SIGNED == x_v.SIGNED) \

module vector_adder 
#(

) (
  input wire        clk,
  input wire        rst_n,
  vector_if.slave   x,
  vector_if.slave   y,
  vector_if.master  sum
);
  
  localparam DATA_WIDTH = x.DATA_WIDTH;
  localparam INDEX_WIDTH = x.INDEX_WIDTH;
  localparam NMASK_WIDTH = x.NMASK_WIDTH;
  
  // For Now just copies X, until vivado is happy

  assign sum.valid = x.valid;
  assign sum.data = x.data;
  assign sum.index = x.index; 
  assign sum.last = x.last;
  assign sum.keep = x.keep;
  
endmodule
