interface axi_lite_if #(
  parameter ADDR_WIDTH  = 5,
  parameter DATA_WIDTH  = 32
);
 
logic [ADDR_WIDTH-1:0]  waddr;  // Address to write to
logic                   wavalid;
logic                   waready;

logic [DATA_WIDTH-1:0]  wdata;  // Data to write
logic                   wvalid; // Data valid
logic                   wready; // Ready to Write 

logic  [DATA_WIDTH-1:0] bdata;  // done writing (optional)
logic                   bvalid;
logic                   bready;

logic [ADDR_WIDTH-1:0]  raddr;    // Address to read from 
logic                   arvalid;  // Address valid
logic                   arready;

logic [DATA_WIDTH-1:0]  rdata;    // data read
logic                   rvalid;   // valid data out
logic                   rready;   // hold on cant accept a read req 

  modport slave (
    input waddr,
    input wavalid,
    output waready, 

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
    output wavalid,
    input waready,

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
