interface vector_ram_if #(
    parameter ADDR_WIDTH  = 5,
    parameter DATA_WIDTH  = 32,
    parameter PARALLELISM = 3
);

    logic [ADDR_WIDTH-1:0]  addr[PARALLELISM-1:0];  // Address to read from
    logic [DATA_WIDTH-1:0]  wdata[PARALLELISM-1:0];  // Data to write
    logic                   write;
    logic                   valid; // Data valid
    logic                   ready; // Ready to Write

    logic  [DATA_WIDTH-1:0] bdata;  // done writing (optional)
    logic                   bvalid;
    logic                   bready;

    logic [DATA_WIDTH-1:0]  rdata[PARALLELISM-1:0];  // data read
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
        input rready // When the slave is responding, it should wait till its master is ready
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
        output rready // When the slave is responding, I can tell it to give me a sec
    );

endinterface
