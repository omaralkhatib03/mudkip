
interface axi_stream_if #(
  parameter DATA_WIDTH  = 32,
  parameter PARALLELISM = 4,
  localparam TOTAL_WIDTH = DATA_WIDTH * PARALLELISM
);

    logic [PARALLELISM-1:0][DATA_WIDTH-1:0] data;
    logic                                   valid;
    logic                                   ready;
    logic                                   last;
    logic [PARALLELISM-1:0]                 mask;

    modport slave (
        input   data,
        input   last,
        input   valid,
        input   mask,
        output  ready
    );

    modport master (
        output  data,
        output  valid,
        output  last,
        output  mask,
        input   ready
    );

endinterface
