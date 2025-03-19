`timescale 1ns/1ps

module vector_ping_pong_ld_wrapper (
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
    function automatic void copy_vector_ram_signals(
        input logic [x.PARALLELISM-1:0]     src_write,
        input logic [x.PARALLELISM-1:0]     src_rready,
        input logic [x.PARALLELISM-1:0]     src_valid,
        input logic [x.ADDR_WIDTH-1:0]      src_addr [x.PARALLELISM-1:0],
        input logic [x.DATA_WIDTH-1:0]      src_wdata [x.PARALLELISM-1:0],

        output logic [x.PARALLELISM-1:0]    dest_write,
        output logic [x.PARALLELISM-1:0]    dest_rready,
        output logic [x.PARALLELISM-1:0]    dest_valid,
        output logic [x.ADDR_WIDTH-1:0]     dest_addr [x.PARALLELISM-1:0],
        output logic [x.DATA_WIDTH-1:0]     dest_wdata [x.PARALLELISM-1:0]
    );
        dest_write  = src_write;
        dest_rready = src_rready;
        dest_valid  = src_valid;

        for (int i = 0; i < x.PARALLELISM; i++) begin
            dest_addr[i]  = src_addr[i];
            dest_wdata[i] = src_wdata[i];
        end
    endfunction

    vector_ram_if #(
        .LENGTH(x.LENGTH),
        .DATA_WIDTH(x.DATA_WIDTH),
        .PARALLELISM(x.PARALLELISM)
    ) ping_vec_i();

    vector_ram_if #(
        .LENGTH(x.LENGTH),
        .DATA_WIDTH(x.DATA_WIDTH),
        .PARALLELISM(x.PARALLELISM)
    ) pong_vec_i();


    always_comb begin
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

    vector_ping_pong iterates_ping_pong_I (
        .clk        (clk),
        .rst_n      (rst_n),

        .ping       (ping),

        .x          (ping_vec_i),
        .x_n        (pong_vec_i),

        .rom_x      (rom_x),
        .rom_x_n    (rom_x_n)
    );

endmodule
