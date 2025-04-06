`timescale 1ns/1ps

module vector_ram_tb  #(
    parameter   NUMBER_OF_RAMS /*verilator public*/ =  2,
    parameter   RAM_FIFO_DEPTH /*verilator public*/ =  4,
    parameter   VECTOR_LENGTH  /*verilator public*/ = 32, 
    parameter   DATA_WIDTH     /*verilator public*/ = 32,
    parameter   PARALLELISM    /*verilator public*/ = 4,
    localparam  ADDR_WIDTH     /*verilator public*/ = $clog2(VECTOR_LENGTH)
) (
    input wire                                      clk,
    input wire                                      rst_n,

    input wire [PARALLELISM-1:0][ADDR_WIDTH-1:0]    addr,
    input wire [PARALLELISM-1:0][DATA_WIDTH-1:0]    wdata,
    input wire                                      write,
    input wire                                      valid,

    output logic                                    ready,
    output logic [PARALLELISM-1:0][DATA_WIDTH-1:0]  rdata,
    output logic                                    rvalid,

    input wire                                      rready
);

    integer i;   

    vector_ram_if #(
       .LENGTH      (VECTOR_LENGTH),
       .DATA_WIDTH  (DATA_WIDTH),
       .PARALLELISM (PARALLELISM)
    ) vec_i          ();

    always@(*) 
    begin
       vec_i.write     = write;
       vec_i.rready    = rready;

       vec_i.valid     = valid;
       ready           = vec_i.ready;

       rvalid          = vec_i.rvalid;

       for (i = 0; i < PARALLELISM; i = i + 1)
       begin
           rdata[i]        = vec_i.rdata[i];
           vec_i.wdata[i]  = wdata[i];
           vec_i.addr[i]   = addr[i];
       end

    end

    vector_ram dut_I (
       .clk(clk),
       .rst_n(rst_n),
       .req(vec_i),
       .rom(vec_i)
    );
    
endmodule

