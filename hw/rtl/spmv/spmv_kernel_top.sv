`timescale 1ns/1ps

// TODO: Add fifos

module spmv_kernel_top #(
    parameter   VECTOR_LENGTH  /*verilator public*/ = 32,
    parameter   DATA_WIDTH     /*verilator public*/ = 32,
    parameter   PARALLELISM    /*verilator public*/ = 4,
    parameter   RELEASE_MODE   /*verilator public*/ = 0,
    parameter   FLOAT                               = 0,
    parameter   E_WIDTH                             = 8,
    parameter   FRAC_WIDTH                          = 23,
    parameter   NUMBER_OF_RAMS                      = 4,
    parameter   RAM_FIFO_DEPTH                      = 4,
    // verilator lint_off UNUSEDPARAM
    localparam  ADDR_WIDTH                          = $clog2(VECTOR_LENGTH)
    // verilator lint_on UNUSEDPARAM
) (
    input wire      clk,
    input wire      rst_n,

    input wire      ping,
    input wire      en,
    output logic    done,

    axi_stream_if.slave val,
    axi_stream_if.slave r_beg,
    axi_stream_if.slave c_idx,

    // Dangerous Interface, Used for testing only. Unused in Release Mode
    // verilator lint_off UNUSED
    input wire          cfg_en,
    vector_ram_if.slave cfg,
    // verilator lint_on UNUSED

    vector_ram_if.slave rom_x,
    vector_ram_if.slave rom_x_n
);

    vector_ram_if #(
        .LENGTH         (VECTOR_LENGTH),
        .DATA_WIDTH     (DATA_WIDTH),
        .PARALLELISM    (PARALLELISM),
        .FLOAT          (FLOAT),
        .E_WIDTH        (E_WIDTH),
        .FRAC_WIDTH     (FRAC_WIDTH) // + implicit 1
    ) x_i();

    vector_ram_if #(
        .LENGTH         (VECTOR_LENGTH),
        .DATA_WIDTH     (DATA_WIDTH),
        .PARALLELISM    (PARALLELISM),
        .FLOAT          (FLOAT),
        .E_WIDTH        (E_WIDTH),
        .FRAC_WIDTH     (FRAC_WIDTH) // + implicit 1

    ) x_n_i();

    spmv_kernel #(
        .LENGTH         (VECTOR_LENGTH),
        .DATA_WIDTH     (DATA_WIDTH),
        .PARALLELISM    (PARALLELISM),
        .FLOAT          (FLOAT),
        .E_WIDTH        (E_WIDTH),
        .FRAC_WIDTH     (FRAC_WIDTH) // + implicit 1
    ) spmv_k_I (
        .clk    (clk),
        .rst_n  (rst_n),
        .en     (en),
        .done   (done),

        .val    (val),
        .r_beg  (r_beg),
        .c_idx  (c_idx),

        .x      (x_i),
        .x_n    (x_n_i)
    );

    vector_ram_if #(
        .LENGTH         (VECTOR_LENGTH),
        .DATA_WIDTH     (DATA_WIDTH),
        .PARALLELISM    (PARALLELISM),
        .FLOAT          (FLOAT),
        .E_WIDTH        (E_WIDTH),
        .FRAC_WIDTH     (FRAC_WIDTH) // + implicit 1
    ) ping_vec_i();

    vector_ram_if #(
        .LENGTH         (VECTOR_LENGTH),
        .DATA_WIDTH     (DATA_WIDTH),
        .PARALLELISM    (PARALLELISM),
        .FLOAT          (FLOAT),
        .E_WIDTH        (E_WIDTH),
        .FRAC_WIDTH     (FRAC_WIDTH) // + implicit 1
    ) pong_vec_i();

    generate
        if (RELEASE_MODE)
        begin : release_mode_gen

            vector_ping_pong #(
                .NUMBER_OF_RAMS (NUMBER_OF_RAMS),
                .RAM_FIFO_DEPTH (RAM_FIFO_DEPTH),
                .LENGTH         (VECTOR_LENGTH),
                .DATA_WIDTH     (DATA_WIDTH),
                .PARALLELISM    (PARALLELISM)
            ) iterates_ping_pong_I (
                .clk        (clk),
                .rst_n      (rst_n),

                .ping       (ping),

                .x          (x_i),
                .x_n        (x_n_i),

                .rom_x      (rom_x),
                .rom_x_n    (rom_x_n)
            );

            vector_ram_slave_null null_I (cfg);

        end
        else
        begin : test_mode_gen

            vector_ping_pong_ld_wrapper #(
                .NUMBER_OF_RAMS (NUMBER_OF_RAMS),
                .RAM_FIFO_DEPTH (RAM_FIFO_DEPTH),
                .LENGTH         (VECTOR_LENGTH),
                .DATA_WIDTH     (DATA_WIDTH),
                .PARALLELISM    (PARALLELISM)
            ) iterates_ping_pong_I (
                .clk        (clk),
                .rst_n      (rst_n),
                .cfg_en     (cfg_en),

                .ping       (ping),

                .x          (x_i),
                .x_n        (x_n_i),

                .rom_x      (rom_x),
                .rom_x_n    (rom_x_n),
                .cfg        (cfg)
            );

        end

    endgenerate


endmodule;

