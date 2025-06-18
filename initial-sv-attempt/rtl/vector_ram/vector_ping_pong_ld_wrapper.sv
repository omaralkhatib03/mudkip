`timescale 1ns/1ps

module vector_ping_pong_ld_wrapper #(
    parameter NUMBER_OF_RAMS        = 2, // Must be a power of 2, or equal to vector parrallelism
    parameter RAM_FIFO_DEPTH        = 4
) (
    input wire clk,
    input wire rst_n,

    input wire ping,
    input wire cfg_en,

    vector_ram_if.slave x,
    vector_ram_if.slave x_n,

    vector_ram_if.slave rom_x,
    vector_ram_if.slave rom_x_n,

    vector_ram_if.slave cfg
);

    localparam LENGTH       = x.LENGTH;
    localparam DATA_WIDTH   = x.DATA_WIDTH;
    localparam PARALLELISM  = x.PARALLELISM;
    localparam ADDR_WIDTH   = x.ADDR_WIDTH;

    function automatic void copy_vector_ram_signals(
        input logic                         src_write,
        input logic                         src_rready,
        input logic                         src_valid,
        input logic [ADDR_WIDTH-1:0]        src_addr [PARALLELISM-1:0],
        input logic [DATA_WIDTH-1:0]        src_wdata [PARALLELISM-1:0],

        output logic                        dest_write,
        output logic                        dest_rready,
        output logic                        dest_valid,
        output logic [ADDR_WIDTH-1:0]       dest_addr [PARALLELISM-1:0],
        output logic [DATA_WIDTH-1:0]       dest_wdata [PARALLELISM-1:0]
    );
        dest_write  = src_write;
        dest_rready = src_rready;
        dest_valid  = src_valid;

        for (int i = 0; i < PARALLELISM; i++) begin
            dest_addr[i]  = src_addr[i];
            dest_wdata[i] = src_wdata[i];
        end
    endfunction

    vector_ram_if #(
        .LENGTH(LENGTH),
        .DATA_WIDTH(DATA_WIDTH),
        .PARALLELISM(PARALLELISM)
    ) ping_vec_i();

    vector_ram_if #(
        .LENGTH(LENGTH),
        .DATA_WIDTH(DATA_WIDTH),
        .PARALLELISM(PARALLELISM)
    ) pong_vec_i();


    always_comb
    begin
        if (cfg_en && ping)
        begin
            copy_vector_ram_signals(cfg.write, cfg.rready, cfg.valid, cfg.addr, cfg.wdata,
                                    ping_vec_i.write, ping_vec_i.rready, ping_vec_i.valid,
                                    ping_vec_i.addr, ping_vec_i.wdata);

            copy_vector_ram_signals(x_n.write, x_n.rready, x_n.valid, x_n.addr, x_n.wdata,
                                    pong_vec_i.write, pong_vec_i.rready, pong_vec_i.valid,
                                    pong_vec_i.addr, pong_vec_i.wdata);
        end
        else if (cfg_en && !ping)
        begin
            copy_vector_ram_signals(x.write, x.rready, x.valid, x.addr, x.wdata,
                                    ping_vec_i.write, ping_vec_i.rready, ping_vec_i.valid,
                                    ping_vec_i.addr, ping_vec_i.wdata);

            copy_vector_ram_signals(cfg.write, cfg.rready, cfg.valid, cfg.addr, cfg.wdata,
                                    pong_vec_i.write, pong_vec_i.rready, pong_vec_i.valid,
                                    pong_vec_i.addr, pong_vec_i.wdata);
        end
        else
        begin
            copy_vector_ram_signals(x.write, x.rready, x.valid, x.addr, x.wdata,
                                    ping_vec_i.write, ping_vec_i.rready, ping_vec_i.valid,
                                    ping_vec_i.addr, ping_vec_i.wdata);

            copy_vector_ram_signals(x_n.write, x_n.rready, x_n.valid, x_n.addr, x_n.wdata,
                                    pong_vec_i.write, pong_vec_i.rready, pong_vec_i.valid,
                                    pong_vec_i.addr, pong_vec_i.wdata);
        end
    end

    vector_ping_pong #(
        .NUMBER_OF_RAMS     (NUMBER_OF_RAMS),
        .RAM_FIFO_DEPTH     (RAM_FIFO_DEPTH)
    ) iterates_ping_pong_I  (
        .clk                (clk),
        .rst_n              (rst_n),

        .ping               (cfg_en ? 0 : ping),

        .x                  (ping_vec_i),
        .x_n                (pong_vec_i),

        .rom_x              (rom_x),
        .rom_x_n            (rom_x_n)
    );

endmodule
