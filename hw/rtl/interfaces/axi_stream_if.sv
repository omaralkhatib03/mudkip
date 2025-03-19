
interface axi_stream_if #(
  parameter DATA_WIDTH  = 32,
  parameter PARALLELISM = 4
);

  logic [DATA_WIDTH-1:0]  data[PARALLELISM-1:0];
  logic                   valid;
  logic                   ready;
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
