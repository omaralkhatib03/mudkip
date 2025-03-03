`timescale 1ns/1ps

module vector_ram_tb  #(
    parameter NUMBER_OF_RAMS        = 2, // Must be a power of 2, or equal to vectot parrallelism 
    parameter RAM_FIFO_DEPTH        = 4,
    parameter VECTOR_LENGTH         = 1024, // I think this was afiro. This is only needed to figure out how big the rams need to be
    localparam ADDR_WIDTH           = $clog2(VECTOR_LENGTH),
    parameter DATA_WIDTH            = 32,
    parameter PARALLELISM           = 8
) (
    input wire                      clk,
    input wire                      rst_n,

    input wire [ADDR_WIDTH-1:0]     waddr[PARALLELISM-1:0],
    input wire [DATA_WIDTH-1:0]     wdata[PARALLELISM-1:0] ,
    
    input wire                      wvalid,
    output logic                    wready,

    output logic  [DATA_WIDTH-1:0]  bdata,
    output logic                    bvalid,

    /* verilator lint_off UNUSED */
    input wire                      bready,
    /* verilator lint_on UNUSED */

    input wire [ADDR_WIDTH-1:0]     raddr[PARALLELISM-1:0],
    input wire                      arvalid,
    output logic                    arready,

    output logic [DATA_WIDTH-1:0]   rdata[PARALLELISM-1:0],
    output logic                    rvalid,
    input wire                      rready
);

    vector_ram_if #(
        .ADDR_WIDTH(ADDR_WIDTH), 
        .DATA_WIDTH(DATA_WIDTH), 
        .PARALLELISM(PARALLELISM)
    ) vec_i();
    
    always_comb
    begin
        vec_i.wvalid    = wvalid;
        wready          = vec_i.wready;

        vec_i.arvalid   = arvalid;
        arready         = vec_i.arready;
        
        vec_i.rready    = rready;
        rvalid          = vec_i.rvalid;

        for (int i = 0; i < PARALLELISM; i++)
        begin
            rdata[i]        = vec_i.rdata[i];
            vec_i.wdata[i]  = wdata[i];
            vec_i.waddr[i]  = waddr[i];
            vec_i.raddr[i]  = raddr[i];
        end

        bdata   = '0;
        bvalid  = '0;
    end

    vector_ram
    #(
        .NUMBER_OF_RAMS(NUMBER_OF_RAMS),
        .RAM_FIFO_DEPTH(RAM_FIFO_DEPTH),
        .VECTOR_LENGTH(VECTOR_LENGTH)
    ) dut_I (
        .clk(clk),
        .rst_n(rst_n),
        .req(vec_i)
    );

endmodule
