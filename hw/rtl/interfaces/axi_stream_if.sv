
interface axi_stream_if #(
  parameter DATA_WIDTH  = 32
);
 
  logic [DATA_WIDTH-1:0]  data;    // data read
  logic                   valid;   // valid data out
  logic                   ready;   // hold on cant accept a read req 
  logic                   last;

  modport slave (
    input data,
    input last,
    input valid,
    output ready
  );

  modport master (
    output data,
    output valid,
    output last,
    input ready
  );

endinterface
