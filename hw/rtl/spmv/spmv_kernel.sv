`timescale 1ns/1ps

// Ista5dim il ready signal ka shiftout

module spmv_kernel (
    input wire          clk,
    input wire          rst_n,

    input wire          en,
    output logic        done,

    axi_stream_if.slave r_beg,
    axi_stream_if.slave val,
    axi_stream_if.slave c_idx,

    vector_ram_if.master x,
    vector_ram_if.master x_n
);

    localparam LENGTH       = x.LENGTH;
    localparam DATA_WDITH   = x.DATA_WIDTH;
    localparam PARALLELISM  = x.PARALLELISM;

    typedef enum integer {
        IDLE,   // Wait for enable
        BUSY    // Computing
    } spmv_kernel_state_enum;

    spmv_kernel_state_enum      state_b;
    spmv_kernel_state_enum      state_r;

    logic [2*DATA_WDITH-1:0]    acc[PARALLELISM-1:0];

    always_comb
    begin
        state_b = state_r;

        case (state_r)
            IDLE:
            begin
                if (en)
                begin
                    state_b = BUSY;
                end
            end
            BUSY:
            begin
                if (done)
                begin
                    state_b = IDLE;
                end
            end
        endcase

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


    generate
        for (genvar i = 0; i < x.PARALLELISM; i++)
        begin : fmac_gen

            fmac #(
                .FLOAT          (1),
                .EXP_WIDTH      (8),
                .MANTISSA_WIDTH (23),
                .DATA_WIDTH     (DATA_WDITH)
            ) fmac_I (
                .clk            (clk),
                .rst_n          (rst_n),

                .val            (acc[i]),
                .multiplicand   (val.data[i]),

                .in_valid       (),
                .reset          (),
                .in_ready       (),

                .acc            (acc[i]),

                .valid          (),
                .ready          (),
                .done           ()
            );

        end
    endgenerate

endmodule;
