`timescale 1ns/1ps

module pipeline #(
    parameter DATA_WIDTH = 32
) (
    input wire clk,
    input wire rst_n,
    input wire [DATA_WIDTH-1:0] in_data,
    input wire in_valid,
    output logic in_ready,

    output logic [DATA_WIDTH-1:0] out_data,
    output logic out_valid,
    input wire out_ready

);

    typedef enum int {
        IDLE,
        BACK_PRESSURE
    } state_t;

    state_t state_b, state_r;
    logic [DATA_WIDTH-1:0] data_b, data_r;
    logic valid_b, valid_r;

    always_comb
    begin
        state_b = state_r;
        data_b = data_r;
        valid_b = valid_r;

        case (state_r)
            IDLE:
            begin
                if (in_valid && out_ready)
                begin
                    state_b = IDLE;
                    data_b = in_data;
                    valid_b = 1'b1;
                end
                else if (in_valid)
                begin
                    state_b = BACK_PRESSURE;
                    data_b = in_data;
                    valid_b = 1'b1;
                end
                else
                begin
                    valid_b = 1'b0;
                end
            end
            BACK_PRESSURE:
            begin
                if (out_ready && in_valid)
                begin
                    state_b = BACK_PRESSURE;
                    data_b = in_data;
                    valid_b = 1'b1;
                end
                else if (out_ready)
                begin
                    state_b = IDLE;
                    valid_b = 1'b0;
                end
            end
        endcase
    end

    always_ff @(posedge clk)
    begin
        if (!rst_n)
        begin
            state_r <= IDLE;
            valid_r <= 1'b0;
        end
        else
        begin
            state_r <= state_b;
            valid_r <= valid_b;
        end
        data_r <= data_b;
    end

    assign in_ready = ((state_r == IDLE) || (state_r == BACK_PRESSURE)) && out_ready;
    assign out_valid = valid_r;
    assign out_data = data_r;

endmodule
