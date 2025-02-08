`timescale 1 ps/1 ps

/*
* This is an AXI-Stream Based Master-Slave interface to 
* stream vectors between modules
*
* If FLOAT = 1, then the floating point format of is defined via the
* parameters. If FLOAT = 0, then MANTISSA defines where the fixed point is.
* Hence a mantissa of 5 means that the point is on the left of the 5th bit.
*
* The Format is of the vector is defined as follows
*
   [Data_1, Data_2, Data_3 ... Data_NUMBERS]
   [Index_1, Index_2, Index_3 ... Index_NUMBERS]
*
* The Data_1 is the value of the element at index Index_1
*/

interface vector_if #(
  parameter   NUMBERS       = 1,
  parameter   NUMBER_WIDTH  = 32,
  parameter   VECTOR_LENGTH = 32,
  parameter   FLOAT         = 1,
  parameter   MANTISSA      = 23,
  parameter   EXPONENT      = 8,
  parameter   SIGNED        = 1,
  localparam  DATA_WIDTH    = NUMBER_WIDTH * NUMBERS,
  localparam  INDEX_WIDTH   = $clog2(VECTOR_LENGTH) * NUMBERS,
  localparam  NMASK_WIDTH   = $clog2(NUMBERS)
);
  

  logic                       ready;  
  logic                       valid;  

  logic [DATA_WIDTH-1:0]      data;
  logic [INDEX_WIDTH-1:0]     index; 

  logic                       last;
  logic [NMASK_WIDTH-1:0]     keep; 

  modport slave (
    output ready,     
    input valid,
    input data,
    input index,
    input last,
    input keep
  );

  modport master (
    input ready,     
    output valid,
    output data,
    output index,
    output last,
    output keep
  );

endinterface
