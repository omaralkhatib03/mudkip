`timescale 1ns/1ps

// A module called pipeline and a parameter called pipeline. The levels of restartdness are crazy rn
module pipeline #(
    parameter DATA_WIDTH    = 32,
    parameter PIPE_LINE     = 0
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

    generate
        if (PIPE_LINE)
        begin : skid_gen
            typedef enum int { PIPE = 0, SKID = 1 } pipe_state_t;
            pipe_state_t state_b, state_r;
            logic [DATA_WIDTH-1:0] data_r, data_b;
            logic [DATA_WIDTH-1:0] spare_r, spare_b;
            logic valid_r, valid_b;
            logic ready_r, ready_b;
            logic pipe_ready;

            always_comb
            begin
                state_b = state_r;
                spare_b = spare_r;
                data_b = data_r;
                valid_b = valid_r;
                ready_b = ready_r;

                case (state_r)
                    PIPE:
                    begin
                        if (pipe_ready)
                        begin
                            data_b = in_data;
                            valid_b = in_valid;
                            ready_b = 1;
                        end
                        else if (in_valid)
                        begin
                            spare_b = in_data;
                            ready_b = 0;
                            state_b = SKID;
                        end
                    end
                    SKID:
                    begin
                        if (out_ready)
                        begin
                            data_b  = spare_r;
                            valid_b = 1;
                            ready_b = 1;
                            state_b = PIPE;
                        end
                    end
                endcase
            end

            always_ff @(posedge clk)
            begin
                if (!rst_n)
                begin
                    state_r <= PIPE;
                    valid_r <= 0;
                    ready_r <= 0;
                end
                else
                begin
                    state_r <= state_b;
                    valid_r <= valid_b;
                    ready_r <= ready_b;
                end
            end

            always_ff @(posedge clk)
            begin
                data_r  <= data_b;
                spare_r <= spare_b;
            end

            assign pipe_ready   = out_ready || !valid_r;
            assign in_ready     = ready_r;
            assign out_data     = data_r;
            assign out_valid    = valid_r;
        end
        else
        begin : pipe_gen
            logic [DATA_WIDTH-1:0] data_r;
            logic                  valid_r;

            assign in_ready  = !valid_r || out_ready;
            assign out_data  = data_r;
            assign out_valid = valid_r;

            always_ff @(posedge clk) begin
                if (!rst_n) begin
                    valid_r <= 1'b0;
                    data_r  <= '0;
                end else if (in_ready) begin
                    valid_r <= in_valid;
                    data_r  <= in_data;
                end
            end
        end
    endgenerate

endmodule

// `timescale 1ns/1ps

// module pipeline #(
//     parameter DATA_WIDTH = 32
// ) (
//     input wire clk,
//     input wire rst_n,
//     input wire [DATA_WIDTH-1:0] in_data,
//     input wire in_valid,
//     output logic in_ready,

//     output logic [DATA_WIDTH-1:0] out_data,
//     output logic out_valid,
//     input wire out_ready

// );

//     logic fifo_full;

//     /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
//     basic_sync_fifo #(
//         .DATA_WIDTH     (DATA_WIDTH),
//         .DEPTH          (4),
//         .READ_LATENCY   (1)
//     ) input_fifo_I      (
//         .clk            (clk),
//         .rst_n          (rst_n),
//         .din            (in_data),
//         .shift_in       (in_valid),
//         .shift_out      (out_ready),
//         .valid          (out_valid),
//         .dout           (out_data),
//         .full           (fifo_full)
//     );
//     /* verilator lint_on PINMISSING */ // (Over/Under)flow Pins

//     assign in_ready     = !fifo_full;

// endmodule

