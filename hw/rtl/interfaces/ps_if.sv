
interface ps_if #(
  parameter ADDR_WIDTH  = 5,
  parameter DATA_WIDTH  = 32
);

logic [ADDR_WIDTH-1:0]  waddr;  // Address to write to
logic [DATA_WIDTH-1:0]  wdata;  // Data to write
logic                   wvalid; // Data valid

logic                   wready; // Ready to Write 
logic                   wresp;  // done writing (optional)

logic [ADDR_WIDTH-1:0]  raddr;    // Address to read from 

logic                   arvalid;  // Address valid
logic [DATA_WIDTH-1:0]  rdata;    // data read

logic                   rvalid;   // valid data out
logic                   rready;   // hold on cant accept a read req 

int                     node_addr; // Valid on arvalid, or wvalid. 

  modport slave (
    // input node_addr, // Valid on arvalid, or wvalid.

    input waddr,
    input wdata,
    input wvalid,
    
    output wresp,
    output wready,

    input raddr,
    input arvalid,

    output rdata,
    output rvalid,
    input rready // When the slave is responding, it should wait till its master is ready 
  );

  modport master (
    // output node_addr, // Valid on arvalid, or wvalid.

    output waddr,
    output wdata,
    output wvalid,
    
    input wresp,
    input wready,

    output raddr,
    output arvalid,

    input rdata,
    input rvalid,

    output rready // When the slave is responding, I can tell it to give me a sec
  );

endinterface
