`timescale 1ns/1ps

// Ista5dim il ready signal ka shiftout

module spmv_kernel #(
    parameter   LENGTH         /*verilator public*/ = 32,
    parameter   DATA_WIDTH     /*verilator public*/ = 32,
    parameter   PARALLELISM    /*verilator public*/ = 4,
    parameter   FLOAT                               = 0,
    // verilator lint_off UNUSEDPARAM
    parameter   E_WIDTH                             = 8,
    parameter   FRAC_WIDTH                          = 23, // + implicit 1
    // verilator lint_on UNUSEDPARAM
    localparam  ADDR_WIDTH                          = $clog2(LENGTH)
) (
    input wire              clk,
    input wire              rst_n,

    input wire              en,
    output logic            done,

    axi_stream_if.slave     r_beg,  // DMA From Memory
    axi_stream_if.slave     val,    // DMA From Memory
    axi_stream_if.slave     c_idx,  // DMA From Memory

    vector_ram_if.master    x,
    vector_ram_if.master    x_n
);
    import spmv_pkg::spmv_kernel_state_enum;

    localparam ACC_WIDTH        = FLOAT ? DATA_WIDTH : 2*DATA_WIDTH;

    spmv_kernel_state_enum      state_b;
    spmv_kernel_state_enum      state_r;

    logic [DATA_WIDTH-1:0] current_rows_r[PARALLELISM-1:0];
    logic [DATA_WIDTH-1:0] current_rows_b[PARALLELISM-1:0];

    logic [DATA_WIDTH-1:0] row_diff[PARALLELISM-1:1];

    logic multiplicand_valid;
    logic product_ready;

    logic acc_in_ready;
    logic acc_in_valid;

    logic [31:0] acc_data_in[PARALLELISM-1:0];

    assign x.write      = '0;
    assign x_n.write    = 1'b1;

    // verilator lint_off PINMISSING
    product #(
        .FLOAT          (1),
        .DATA_WIDTH     (32),
        .E_WIDTH        (8),
        .FRAC_WIDTH     (23),
        .PARALLELISM    (PARALLELISM),
        .DELAY          (2)
    ) prod_I (

        .clk            (clk),

        .in_valid       (multiplicand_valid),
        .in_ready       (product_ready),

        .a              (x.rdata),
        .b              (val.data),

        .out            (acc_data_in), // This goes into the row reduction network
        .valid          (acc_in_valid),
        .ready          (acc_in_ready) // takes ready from acc network
    );
    // verilator lint_on PINMISSING

    always_comb
    begin
        state_b     = state_r;
        x.rready    = 0;

        for (int i = 1; i < PARALLELISM; i++)
        begin
            row_diff[i]         = current_rows_r[i] - current_rows_r[i - 1];
        end

        for (int i = 0; i < PARALLELISM; i++)
        begin
            x.addr[i]           = ADDR_WIDTH'(c_idx.data[i]);
            current_rows_b[i]   = current_rows_r[i];
        end

        case (state_r)
            IDLE:
            begin
                if (en && r_beg.valid && r_beg.ready)
                begin
                    state_b = BUSY;
                    x.valid = c_idx.valid && x.ready;
                end
            end
            BUSY:
            begin
                if (done)
                begin
                    state_b = IDLE;
                end

                multiplicand_valid  = x.rvalid && product_ready;
                x.rready            = product_ready;


                // else if (!x.ready)
                // begin
                    // Do nothing and wait
                // end

            end
        endcase

    end

    always_ff @(posedge clk)
    begin
        for (int i = 0 ; i < PARALLELISM; i++)
        begin
            current_rows_r[i] <= current_rows_b[i];
        end
    end

    always_ff @(posedge clk)
    begin
        if (!rst_n)
        begin
            state_r <= IDLE;
        end
        else
        begin
            state_r <= state_b;
        end
    end

    always_comb
    begin
        r_beg.ready = 0;

        for (int i = 0; i < PARALLELISM - 1; i++)
        begin
            r_beg.ready = r_beg.ready && (row_diff[i] == 0);
        end
    end

endmodule;
