interface ref_if #(
    localparam ADDR_WIDTH   = $clog2(LENGTH),
    parameter DATA_WIDTH    = 32
);

    logic [ADDR_WIDTH-1:0]  addr;  // Address to read from
    logic [DATA_WIDTH-1:0]  wdata;  // Data to write
    logic                   write;
    logic                   valid; // Data valid
    logic                   ready; // Ready to Write

    logic  [DATA_WIDTH-1:0] bdata;  // done writing (optional)
    logic                   bvalid;
    logic                   bready;

    logic [DATA_WIDTH-1:0]  rdata;  // data read
    logic                   rvalid; // valid data out
    logic                   rready; // hold on cant accept a read req

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

