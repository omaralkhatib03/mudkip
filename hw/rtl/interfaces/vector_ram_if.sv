interface vector_ram_if #(
    parameter ADDR_WIDTH  = 5,
    parameter DATA_WIDTH  = 32,
    parameter PARALLELISM = 3
);

    wire [ADDR_WIDTH-1:0]  waddr[PARALLELISM-1:0];  // Address to write to
    wire [DATA_WIDTH-1:0]  wdata[PARALLELISM-1:0];  // Data to write
    wire                   wvalid; // Data valid
    wire                   wready; // Ready to Write

    wire  [DATA_WIDTH-1:0] bdata;  // done writing (optional)
    wire                   bvalid;
    wire                   bready;

    wire [ADDR_WIDTH-1:0]  raddr[PARALLELISM-1:0];  // Address to read from
    wire                   arvalid;// Address valid
    wire                   arready;

    wire [DATA_WIDTH-1:0]  rdata[PARALLELISM-1:0];  // data read
    wire                   rvalid; // valid data out
    wire                   rready; // hold on cant accept a read req

    modport slave (
        input waddr,
        input wdata,
        input wvalid,
        output wready,

        output bdata,
        output bvalid,
        input bready,

        input raddr,
        input arvalid,
        output arready,

        output rdata,
        output rvalid,
        input rready // When the slave is responding, it should wait till its master is ready
    );

    modport master (
        output waddr,
        output wdata,
        output wvalid,
        input wready,

        input bdata,
        input bvalid,
        output bready,

        output raddr,
        output arvalid,
        input arready,

        input rdata,
        input rvalid,
        output rready // When the slave is responding, I can tell it to give me a sec
    );

endinterface
