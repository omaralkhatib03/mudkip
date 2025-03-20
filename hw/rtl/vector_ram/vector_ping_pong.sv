`timescale 1ns/1ps

module vector_ping_pong #(
    parameter NUMBER_OF_RAMS        = 2, // Must be a power of 2, or equal to vector parrallelism
    parameter RAM_FIFO_DEPTH        = 4,
    parameter LENGTH                = 32,
    parameter DATA_WIDTH            = 32,
    parameter PARALLELISM           = 4,
    localparam ADDR_WIDTH           = $clog2(LENGTH)
) (
    input wire clk,
    input wire rst_n,

    input wire ping,

    vector_ram_if.slave x,
    vector_ram_if.slave x_n,

    vector_ram_if.slave rom_x,
    vector_ram_if.slave rom_x_n

);

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

    logic ping_r;
    logic current_ping;

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

    vector_ram_if #(
        .LENGTH(LENGTH),
        .DATA_WIDTH(DATA_WIDTH),
        .PARALLELISM(PARALLELISM)
    ) rom_ping_vec_i();

    vector_ram_if #(
        .LENGTH(LENGTH),
        .DATA_WIDTH(DATA_WIDTH),
        .PARALLELISM(PARALLELISM)
    ) rom_pong_vec_i();

    always_ff @(posedge clk)
    begin
        if (!rst_n)
        begin
            ping_r <= 0;
        end
        else
        begin
            ping_r <= ping_r ^ ping;
        end
    end


    always_comb
    begin
        current_ping                = ping ? ping_r ^ ping : ping_r;

        if (current_ping)
        begin
            copy_vector_ram_signals(x.write, x.rready, x.valid, x.addr, x.wdata,
                                    pong_vec_i.write, pong_vec_i.rready, pong_vec_i.valid,
                                    pong_vec_i.addr, pong_vec_i.wdata);

            copy_vector_ram_signals(x_n.write, x_n.rready, x_n.valid, x_n.addr, x_n.wdata,
                                    ping_vec_i.write, ping_vec_i.rready, ping_vec_i.valid,
                                    ping_vec_i.addr, ping_vec_i.wdata);

            copy_vector_ram_signals(rom_x_n.write, rom_x_n.rready, rom_x_n.valid, rom_x_n.addr,
                                    rom_x_n.wdata, rom_ping_vec_i.write, rom_ping_vec_i.rready,
                                    rom_ping_vec_i.valid, rom_ping_vec_i.addr, rom_ping_vec_i.wdata);

            copy_vector_ram_signals(rom_x.write, rom_x.rready, rom_x.valid, rom_x.addr,
                                    rom_x.wdata, rom_pong_vec_i.write, rom_pong_vec_i.rready,
                                    rom_pong_vec_i.valid, rom_pong_vec_i.addr, rom_pong_vec_i.wdata);
        end
        else
        begin
            copy_vector_ram_signals(x_n.write, x_n.rready, x_n.valid, x_n.addr, x_n.wdata,
                                    pong_vec_i.write, pong_vec_i.rready, pong_vec_i.valid,
                                    pong_vec_i.addr, pong_vec_i.wdata);

            copy_vector_ram_signals(x.write, x.rready, x.valid, x.addr, x.wdata,
                                    ping_vec_i.write, ping_vec_i.rready, ping_vec_i.valid,
                                    ping_vec_i.addr, ping_vec_i.wdata);

            copy_vector_ram_signals(rom_x.write, rom_x.rready, rom_x.valid, rom_x.addr,
                                    rom_x.wdata, rom_ping_vec_i.write, rom_ping_vec_i.rready,
                                    rom_ping_vec_i.valid, rom_ping_vec_i.addr, rom_ping_vec_i.wdata);

            copy_vector_ram_signals(rom_x_n.write, rom_x_n.rready, rom_x_n.valid, rom_x_n.addr,
                                    rom_x_n.wdata, rom_pong_vec_i.write, rom_pong_vec_i.rready,
                                    rom_pong_vec_i.valid, rom_pong_vec_i.addr, rom_pong_vec_i.wdata);
        end
    end

    vector_ram #(
        .NUMBER_OF_RAMS (NUMBER_OF_RAMS),
        .RAM_FIFO_DEPTH (RAM_FIFO_DEPTH),
        .LENGTH         (LENGTH),
        .DATA_WIDTH     (DATA_WIDTH),
        .PARALLELISM    (PARALLELISM)
    ) ping_ram_I ( // 0th
        .clk    (clk),
        .rst_n  (rst_n),
        .req    (ping_vec_i),
        .rom    (rom_ping_vec_i)
    );

    vector_ram #(
        .NUMBER_OF_RAMS (NUMBER_OF_RAMS),
        .RAM_FIFO_DEPTH (RAM_FIFO_DEPTH),
        .LENGTH         (LENGTH),
        .DATA_WIDTH     (DATA_WIDTH),
        .PARALLELISM    (PARALLELISM)
    ) pong_ram_I ( // 1th
        .clk    (clk),
        .rst_n  (rst_n),
        .req    (pong_vec_i),
        .rom    (rom_pong_vec_i)
    );

endmodule
