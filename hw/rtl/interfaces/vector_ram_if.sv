interface vector_ram_if #(
    parameter LENGTH        = 32,
    localparam ADDR_WIDTH   = $clog2(LENGTH),
    parameter DATA_WIDTH    = 32,
    parameter PARALLELISM   = 4
);

    logic [ADDR_WIDTH-1:0]      addr [PARALLELISM-1:0];  // Address to read from
    logic [DATA_WIDTH-1:0]      wdata[PARALLELISM-1:0];  // Data to write
    logic [PARALLELISM-1:0]     write;
    logic [PARALLELISM-1:0]     valid; // Data valid
    logic [PARALLELISM-1:0]     ready; // Ready to Write

    logic  [DATA_WIDTH-1:0]     bdata;  // done writing (optional)
    logic  [PARALLELISM-1:0]    bvalid;
    logic  [PARALLELISM-1:0]    bready;

    logic [DATA_WIDTH-1:0]      rdata [PARALLELISM-1:0];  // data read
    logic [PARALLELISM-1:0]     rvalid; // valid data out
    logic [PARALLELISM-1:0]     rready; // hold on cant accept a read req

    modport slave (
        input addr,
        input wdata,
        input write,
        output ready,
        input valid,

        output bdata,
        output bvalid,
        input bready,

        output rdata,
        output rvalid,
        input rready
    );

    modport master (
        output addr,
        output wdata,
        output write,
        output valid,
        input ready,

        input bdata,
        input bvalid,
        output bready,

        input rdata,
        input rvalid,
        output rready
    );

endinterface
