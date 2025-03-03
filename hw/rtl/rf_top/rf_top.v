`timescale 1ns/1ps

module rf_top    #(
    parameter DATA_WIDTH     /* verilator public */ = 32,
    parameter ADDR_WIDTH     /* verilator public */ = 4
) (
    input wire                                     clk,
    input wire                                     rst_n,

    // AXI Master PS
    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWADDR" *)
    input wire [ADDR_WIDTH-1:0] waddr,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWVALID" *)
    input wire                                    wavalid,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWREADY" *)
    output wire                                     waready,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WDATA" *)
    input wire [DATA_WIDTH-1:0] wdata,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WVALID" *)
    input wire                                    wvalid,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WREADY" *)
    output wire                             wready,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BRESP" *)
    output wire [DATA_WIDTH-1:0]    wresp,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BVALID" *)
    output wire                                     bvalid,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BREADY" *)
    input wire                                    bready,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARADDR" *)
    input wire [ADDR_WIDTH-1:0] raddr,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARVALID" *)
    input wire                                    arvalid,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARREADY" *)
    output wire                                    arready,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RDATA" *)
    output wire    [DATA_WIDTH-1:0] rdata,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RVALID" *)
    output wire                                     rvalid,

    (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RREADY" *)
    input wire                                    rready

);

    axi_lite_if #(
        .ADDR_WIDTH(ADDR_WIDTH),
        .DATA_WIDTH(DATA_WIDTH)
    ) axil_ps_if ();

    assign axil_ps_if.waddr        = waddr;
    assign axil_ps_if.wavalid    = wavalid;
    assign waready                         = axil_ps_if.waready;

    assign axil_ps_if.wdata        = wdata;
    assign axil_ps_if.wvalid     = wvalid;
    assign wready                            = axil_ps_if.wready;

    assign wresp                             = axil_ps_if.bdata;
    assign bvalid                            = axil_ps_if.bvalid;
    assign axil_ps_if.bready     = bready;

    assign axil_ps_if.raddr        = raddr;
    assign axil_ps_if.arvalid    = arvalid;
    assign arready                         = axil_ps_if.arready;

    assign rdata                             = axil_ps_if.rdata;
    assign rvalid                            = axil_ps_if.rvalid;
    assign axil_ps_if.rready     = rready;

    // ps_if #(
    //     .ADDR_WIDTH(axil_ps_if.ADDR_WIDTH),
    //     .DATA_WIDTH(axil_ps_if.DATA_WIDTH)
    // ) ps_m_i ();

    //  axl_ps_adapter    #(
    //     .FIFO_DEPTH(64)
    // ) axl_ps_top_adapter_I (
    //     .clk                (clk),
    //     .rst_n            (rst_n),
    //     .axl_m_i        (axil_ps_if),
    //     .ps_m_i         (ps_m_i)
    // );

    // rf_node dut_I (
    //     .clk        (clk),
    //     .rst_n    (rst_n),
    //     .ps_i     (ps_m_i)
    // );

endmodule

