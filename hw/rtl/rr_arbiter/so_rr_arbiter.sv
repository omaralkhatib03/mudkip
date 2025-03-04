`timescale 1ns/1ps

`timescale 1ns/1ps

module so_rr_arbiter #(
  parameter NUM_INPUTS    = 4,
  parameter DATA_WIDTH    = 8,
  parameter FIFO_DEPTH    = 8
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

    logic [NUM_INPUTS-1:0] round_robin_ptr;
    logic [NUM_INPUTS-1:0] grant;
    logic [NUM_INPUTS-1:0] req_masked;
    logic [DATA_WIDTH-1:0] fifo_out[NUM_INPUTS-1:0];
    logic [NUM_INPUTS-1:0] fifo_valid;
    logic [NUM_INPUTS-1:0] fifo_full;
    
    assign in_ready         = ~fifo_full;

    genvar i;
    generate

        for (i = 0; i < NUM_INPUTS; i = i + 1) 
        begin : input_fifo_gen

            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
            basic_sync_fifo #(
                .DATA_WIDTH     (DATA_WIDTH),
                .DEPTH          (FIFO_DEPTH),
                .READ_LATENCY   (1)
            ) input_fifo_I      (
                .clk            (clk),
                .rst_n          (rst_n),
                .din            (in[i]),
                .shift_in       (req[i]),
                .shift_out      (grant[i] & ready),
                .valid          (fifo_valid[i]),
                .dout           (fifo_out[i]),
                .full           (fifo_full[i])
            );
            /* verilator lint_on PINMISSING */ // (Over/Under)flow Pins

        end
    endgenerate
    
    always_ff @(posedge clk) 
    begin
        if (!rst_n) 
        begin
            round_robin_ptr <= 1;
        end 
        else if (valid && ready) 
        begin
            round_robin_ptr <= {round_robin_ptr[NUM_INPUTS-2:0], round_robin_ptr[NUM_INPUTS-1]};
        end
    end
    
    assign req_masked = fifo_valid & round_robin_ptr;
    assign grant = (|req_masked) ? req_masked : fifo_valid;
    
    integer j;

    always_comb 
    begin
        valid = 0;
        dout = '0;

        for (j = 0; j < NUM_INPUTS; j++) 
        begin
            if (grant[j]) 
            begin
                valid   = 1;
                dout    = fifo_out[j];
            end
        end
    end

endmodule

// module rr_arbiter #(
//   parameter NUM_INPUTS    = 4,
//   parameter DATA_WIDTH    = 8,
//   parameter FIFO_DEPTH      = 8
// ) (
//   input   wire                      clk,
//   input   wire                      rst_n,
// 
//   input   wire [NUM_INPUTS-1:0]     req,
//   input   wire [DATA_WIDTH-1:0]     in[NUM_INPUTS-1:0],
// 
//   output logic                      valid,
//   output logic  [DATA_WIDTH-1:0]    dout,
//   input  wire                       ready
// );
// 
//     function automatic logic [NUM_INPUTS-1:0] getInputTurn(
//         logic [NUM_INPUTS-1:0] base,
//         logic [NUM_INPUTS-1:0] req_calc
//     );
//         logic [NUM_INPUTS-1:0] grant;
//         logic [2*NUM_INPUTS-1:0] double_req;
//         logic [2*NUM_INPUTS-1:0] double_grant;
// 
//         // https://stackoverflow.com/questions/55015328/understanding-a-simple-round-robin-arbiter-verilog-code
//         double_req ={req_calc,req_calc};
//         double_grant = double_req & ~(double_req-(NUM_INPUTS * 2)'(base));
//         grant = double_grant[NUM_INPUTS-1:0] | double_grant[2*NUM_INPUTS-1:NUM_INPUTS];
// 
//         return grant;
//     endfunction
//     
//     genvar i;
// 
//     generate
//         
//         for (i = 0; i < NUM_INPUTS; i++)
//         begin : input_fifo_gen
//             basic_sync_fifo #(
//                 .DATA_WIDTH   (DATA_WIDTH),
//                 .DEPTH        (FIFO_DEPTH),
//                 .READ_LATENCY (0)
//             ) input_fifo_I (
//                 .clk          (clk),
//                 .rst_n        (rst_n),
// 
//                 .din          (in[i]),
//                 .shift_in     (req[i]),
// 
//                 .shift_out    (),
//                 .valid        (),
//                 .dout         (),
//                 .empty        (),
//                 .full         ()
//             );
//         end
// 
//     endgenerate
// 
// endmodule



