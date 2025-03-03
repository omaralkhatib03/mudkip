`timescale 1ns/1ps

module rf_node (
    input clk,
    input rst_n,

    reg_if.slave reg_i
);

    localparam NUMBER_OF_PS_REGISTERS = 2**reg_i.ADDR_WIDTH;
    localparam dma_axi_addr = 0, dma_axi_valid = 1;

    logic    [31:0] mem[NUMBER_OF_PS_REGISTERS-1:0];
    logic    [31:0] mem_b[NUMBER_OF_PS_REGISTERS-1:0];

    logic bvalid;

    always_comb
    begin
        for (int i = 0; i < NUMBER_OF_PS_REGISTERS; i++)
        begin
            mem_b[i] = mem[i];
        end

        if (reg_i.wvalid)
        begin
            mem_b[reg_i.waddr] = reg_i.wdata;
        end
    end

    // Write logic
    always_ff @(posedge clk)
    begin
        if (!rst_n)
        begin
            mem[dma_axi_valid] <= '0; // Reset important registers, not all of them
        end
        else
        begin
            mem                 <= mem_b;
            bvalid            <= reg_i.wvalid;
        end
    end

    assign reg_i.wready    = '1;
    assign reg_i.aready    = '1;
    assign reg_i.rvalid    = reg_i.arvalid;
    assign reg_i.rdata     = reg_i.arvalid ? mem[reg_i.raddr] : '0;
    assign reg_i.bvalid    = bvalid;

endmodule;

