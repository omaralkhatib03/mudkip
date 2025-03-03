
interface reg_if #(
    parameter ADDR_WIDTH  = 5,
    parameter DATA_WIDTH  = 32
);

    wire [ADDR_WIDTH-1:0]  waddr;  // Address to write to
    wire [DATA_WIDTH-1:0]  wdata;  // Data to write
    wire                   wvalid; // Data valid
    wire                   wready; // Ready to Write

    wire                   bready; // Ready to Write
    wire  [DATA_WIDTH-1:0] bdata;  // done writing (optional)
    wire                   bvalid; // done writing (optional)

    wire [ADDR_WIDTH-1:0]  raddr;   // Address to read from
    wire                   arvalid; // Address valid
    wire                   aready;  // Address valid

    wire [DATA_WIDTH-1:0]  rdata;   // data read
    wire                   rvalid;  // valid data out
    wire                   rready;  // hold on cant accept a read req

  modport slave (
      input waddr,
      input wdata,
      input wvalid,
      output wready,

      input bready,
      output bdata,
      output bvalid,

      input raddr,
      input arvalid,
      output aready,

      output rdata,
      output rvalid,
      input rready // When the slave is responding, it should wait till its master is ready
    );

    modport master (
      output waddr,
      output wdata,
      output wvalid,
      input wready,

      output bready,
      input bdata,
      input bvalid,

      output raddr,
      output arvalid,
      input aready,

      input rdata,
      input rvalid,
      output rready // When the slave is responding, I can tell it to give me a sec
      );

    endinterface

