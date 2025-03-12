`timescale 1ns/1ps

module so_rr_arbiter #(
  parameter NUM_INPUTS /*verilator public*/  =   4,
  parameter DATA_WIDTH /*verilator public*/  =   8,
  parameter FIFO_DEPTH /*verilator public*/  =   8
) (
  input   wire                      clk,
  input   wire                      rst_n,

  input   wire [NUM_INPUTS-1:0]     req,
  input   wire [DATA_WIDTH-1:0]     in[NUM_INPUTS-1:0],
  output logic  [NUM_INPUTS-1:0]    in_ready,

  output  logic                     valid,
  output  logic [DATA_WIDTH-1:0]    dout,
  input   wire                      ready
);

    logic [DATA_WIDTH-1:0]          fifo_out[NUM_INPUTS-1:0];
    logic [NUM_INPUTS-1:0]          fifo_full;
    logic [NUM_INPUTS-1:0]          fifo_valid;
    logic [NUM_INPUTS-1:0]          fifo_req;
    
    logic [NUM_INPUTS-1:0]          fifo_shift_out;
    logic [NUM_INPUTS-1:0]          shift_in;

    // verilator lint_off UNUSED    
    logic [NUM_INPUTS-1:0]          fifo_empty;
    // verilator lint_on UNUSED

    logic [NUM_INPUTS-1:0]          grant;
    logic [NUM_INPUTS-1:0]          prev_req;

    logic [NUM_INPUTS-1:0]          base_b;
    logic [NUM_INPUTS-1:0]          base_r;
    logic                           idle;
    logic [2*NUM_INPUTS-1:0]        double_req;
    logic [2*NUM_INPUTS-1:0]        double_grant;

    assign fifo_req                 = ~fifo_empty;
    assign double_req               = {fifo_req, fifo_req}; 
    assign in_ready                 = ~fifo_full;
    assign idle                     = fifo_req == 0;
    assign double_grant             = idle ? 0 : double_req & ~(double_req - {{(NUM_INPUTS){1'b0}}, base_r});
    assign grant                    = double_grant[NUM_INPUTS-1:0] | double_grant[2*NUM_INPUTS-1:NUM_INPUTS];

    always_comb
    begin
        base_b                      = base_r;

        if (|grant) 
        begin
            base_b = {base_r[NUM_INPUTS-2:0], base_r[NUM_INPUTS-1]}; // Rotate left instead of shifting up
        end

    end

    always_ff @(posedge clk)
    begin
        if (!rst_n) 
        begin
            base_r          <= 1;
        end
        else 
        begin
            prev_req        <= fifo_req;
            base_r          <= base_b;
        end
    end

    genvar i;
    generate

        for (i = 0; i < NUM_INPUTS; i = i + 1) 
        begin : input_fifo_gen
            assign shift_in[i] = req[i] && in_ready[i];

            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
            basic_sync_fifo #(
                .DATA_WIDTH     (DATA_WIDTH),
                .DEPTH          (FIFO_DEPTH),
                .READ_LATENCY   (1)
            ) input_fifo_I      (
                .clk            (clk),
                .rst_n          (rst_n),
                .din            (in[i]),
                .shift_in       (shift_in[i]),
                .shift_out      (fifo_shift_out[i]),
                .valid          (fifo_valid[i]),
                .dout           (fifo_out[i]),
                .full           (fifo_full[i]),
                .empty          (fifo_empty[i])

            );
            /* verilator lint_on PINMISSING */ // (Over/Under)flow Pins

        end
    endgenerate


    always_comb 
    begin
        valid = 0;
        dout = '0;

        for (int j = 0; j < NUM_INPUTS; j++) 
        begin
            fifo_shift_out[j] = grant[j] && ready && !fifo_empty[j];

            if (fifo_shift_out[j]) 
            begin
                valid   = 1;
                dout    = fifo_out[j];
            end
        end
    end

endmodule
