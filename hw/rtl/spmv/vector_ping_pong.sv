`timescale 1ns/1ps

module vector_ping_pong (
    input wire clk,
    input wire rst_n,

    input wire ping,

    vector_ram_if.slave x,
    vector_ram_if.slave x_n,

    vector_ram_if.slave rom_x,
    vector_ram_if.slave rom_x_n

);

localparam LENGTH       = x.LENGTH;
localparam DATA_WIDTH   = x.DATA_WIDTH;
localparam PARALLELISM  = x.PARALLELISM;

logic ping_r;

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
        if (ping)
        begin
            ping_r <= !ping_r;
        end
    end
end

always_comb
begin
    if (ping_r)
    begin
        ping_vec_i.write        = x_n.write;
        ping_vec_i.rready       = x_n.rready;
        ping_vec_i.valid        = x_n.valid;
        x_n.ready               = ping_vec_i.ready;
        x_n.rvalid              = ping_vec_i.rvalid;

        rom_ping_vec_i.write    = rom_x_n.write;
        rom_ping_vec_i.rready   = rom_x_n.rready;
        rom_ping_vec_i.valid    = rom_x_n.valid;
        rom_x_n.ready           = rom_ping_vec_i.ready;
        rom_x_n.rvalid          = rom_ping_vec_i.rvalid;

        pong_vec_i.write        = x.write;
        pong_vec_i.rready       = x.rready;
        pong_vec_i.valid        = x.valid;
        x.ready                 = pong_vec_i.ready;
        x.rvalid                = pong_vec_i.rvalid;

        rom_pong_vec_i.write    = rom_x.write;
        rom_pong_vec_i.rready   = rom_x.rready;
        rom_pong_vec_i.valid    = rom_x.valid;
        rom_x.ready             = rom_pong_vec_i.ready;
        rom_x.rvalid            = rom_pong_vec_i.rvalid;

        for (int i = 0; i < PARALLELISM; i++)
        begin
            x_n.rdata[i]            = ping_vec_i.rdata[i];
            ping_vec_i.wdata[i]     = x_n.wdata[i];
            ping_vec_i.addr[i]      = x_n.addr[i];

            rom_x_n.rdata[i]        = rom_ping_vec_i.rdata[i];
            rom_ping_vec_i.wdata[i] = rom_x_n.wdata[i];
            rom_ping_vec_i.addr[i]  = rom_x_n.addr[i];

            x.rdata[i]              = pong_vec_i.rdata[i];
            pong_vec_i.wdata[i]     = x.wdata[i];
            pong_vec_i.addr[i]      = x.addr[i];

            rom_x.rdata[i]          = rom_pong_vec_i.rdata[i];
            rom_pong_vec_i.wdata[i] = rom_x.wdata[i];
            rom_pong_vec_i.addr[i]  = rom_x.addr[i];
        end
    end
    else
    begin
        pong_vec_i.write        = x_n.write;
        pong_vec_i.rready       = x_n.rready;
        pong_vec_i.valid        = x_n.valid;
        x_n.ready               = pong_vec_i.ready;
        x_n.rvalid              = pong_vec_i.rvalid;

        rom_pong_vec_i.write    = rom_x_n.write;
        rom_pong_vec_i.rready   = rom_x_n.rready;
        rom_pong_vec_i.valid    = rom_x_n.valid;
        rom_x_n.ready           = rom_pong_vec_i.ready;
        rom_x_n.rvalid          = rom_pong_vec_i.rvalid;

        ping_vec_i.write        = x.write;
        ping_vec_i.rready       = x.rready;
        ping_vec_i.valid        = x.valid;
        x.ready                 = ping_vec_i.ready;
        x.rvalid                = ping_vec_i.rvalid;

        rom_ping_vec_i.write    = rom_x.write;
        rom_ping_vec_i.rready   = rom_x.rready;
        rom_ping_vec_i.valid    = rom_x.valid;
        rom_x.ready             = rom_ping_vec_i.ready;
        rom_x.rvalid            = rom_ping_vec_i.rvalid;

        for (int i = 0; i < PARALLELISM; i++)
        begin
            x_n.rdata[i]                = pong_vec_i.rdata[i];
            pong_vec_i.wdata[i]         = x_n.wdata[i];
            pong_vec_i.addr[i]          = x_n.addr[i];

            x.rdata[i]                  = ping_vec_i.rdata[i];
            ping_vec_i.wdata[i]         = x.wdata[i];
            ping_vec_i.addr[i]          = x.addr[i];

            rom_x_n.rdata[i]            = rom_pong_vec_i.rdata[i];
            rom_pong_vec_i.wdata[i]     = rom_x_n.wdata[i];
            rom_pong_vec_i.addr[i]      = rom_x_n.addr[i];

            rom_x.rdata[i]              = rom_ping_vec_i.rdata[i];
            rom_ping_vec_i.wdata[i]     = rom_x.wdata[i];
            rom_ping_vec_i.addr[i]      = rom_x.addr[i];
        end
    end
end

vector_ram ping_ram_I ( // 0th
    .clk    (clk),
    .rst_n  (rst_n),
    .req    (ping_vec_i),
    .rom    (rom_ping_vec_i)
);

vector_ram pong_ram_I ( // 1th
    .clk    (clk),
    .rst_n  (rst_n),
    .req    (pong_vec_i),
    .rom    (rom_pong_vec_i)
);

endmodule
