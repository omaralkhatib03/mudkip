`timescale 1ns/1ps

module vector_ram_tb  #(
    parameter   NUMBER_OF_RAMS /*verilator public*/ = 4, // Must be a power of 2, or equal to vector parrallelism
    parameter   RAM_FIFO_DEPTH /*verilator public*/ = 4,
    parameter   VECTOR_LENGTH  /*verilator public*/ = 32, // I think this was afiro. This is only needed to figure out how big the rams need to be
    parameter   DATA_WIDTH     /*verilator public*/ = 32,
    parameter   PARALLELISM    /*verilator public*/ = 4,
    localparam  ADDR_WIDTH     /*verilator public*/ = $clog2(VECTOR_LENGTH)
) (
    input wire                      clk,
    input wire                      rst_n,

    input wire [ADDR_WIDTH-1:0]     addr[PARALLELISM-1:0],
    input wire [DATA_WIDTH-1:0]     wdata[PARALLELISM-1:0] ,
    input wire                      write,
    input wire                      valid,
    output logic                    ready,
    output logic [DATA_WIDTH-1:0]   rdata[PARALLELISM-1:0],
    output logic                    rvalid,
    input wire                      rready
);

//    vector_ram_if #(
//        .LENGTH(VECTOR_LENGTH),
//        .DATA_WIDTH(DATA_WIDTH),
//        .PARALLELISM(PARALLELISM)
//    ) vec_i();
//
//    always_comb
//    begin
//        vec_i.write     = write;
//        vec_i.rready    = rready;
//
//        vec_i.valid     = valid;
//        ready           = vec_i.ready;
//
//        rvalid          = vec_i.rvalid;
//
//        for (int i = 0; i < PARALLELISM; i++)
//        begin
//            rdata[i]        = vec_i.rdata[i];
//            vec_i.wdata[i]  = wdata[i];
//            vec_i.addr[i]   = addr[i];
//        end
//
//    end
//
//    vector_ram
//    #(
//        .NUMBER_OF_RAMS(NUMBER_OF_RAMS),
//        .RAM_FIFO_DEPTH(RAM_FIFO_DEPTH)
//    ) dut_I (
//        .clk(clk),
//        .rst_n(rst_n),
//        .req(vec_i),
//        .rom(vec_i)
//    );

endmodule
