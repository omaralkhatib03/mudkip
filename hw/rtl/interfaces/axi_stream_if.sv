
interface axi_stream_if #(
  parameter DATA_WIDTH  = 32,
  parameter PARALLELISM = 4
);

    logic [DATA_WIDTH-1:0]    data[PARALLELISM-1:0];
    logic                     valid;
    logic                     ready;
    logic                     last;
    logic                     bytemask;

    modport slave (
        input   data,
        input   last,
        input   valid,
        input   bytemask,
        output  ready
    );

    modport master (
        output  data,
        output  valid,
        output  last,
        output  bytemask,
        input   ready
    );

endinterface
