`timescale 1ns/1ps

/*
*  Fast Vector Ram should allow for random access to vector data -> Used in SpMV
*
*  This will consume quite a bit of (B/U)RAM for larger problems
*  E.g Assume we are working with single precision numbers (32 bits),
*  a 512 compute bus would require 18 BRAMS in parallel, which is to be
*  increased if we require more memory. I.e does not fit in one BRAM.
*
*  A single BRAM configured for 32 bit width stores 1Kb, which is 512 numbers.
*/

/*
*   Design:
*   If I want to actually utilise the parallelsim function on chip, I need to store vectors efficiently.
*   A vector ram allows for reading and writing vectors into off-chip memory. The main feature is seperating
*   the parallelism parameter from the number of (B/U)RAMS needed in parallel. For example, you can instantiate
*   a 256 bit bus to process 8 single precision numbers at a time with 4x32 bit RAMS in parallel.
*   This affects throughput of course but we can uses FIFO's to virtually 'maintain' a throughput of one on the input side.
*/

module vector_ram
#(
    parameter NUMBER_OF_RAMS        = 2, // Must be a power of 2, or equal to vectot parrallelism
    parameter RAM_FIFO_DEPTH        = 4,
    parameter VECTOR_LENGTH         = 32 // I think this was afiro
) (
    input         wire              clk,
    input         wire              rst_n,
    vector_ram_if.slave             req
);

    localparam PORTS                = req.PARALLELISM; // Virtual Ports
    localparam DATA_WIDTH           = req.DATA_WIDTH;
    localparam ADDR_WIDTH           = req.ADDR_WIDTH;
    localparam PORT_PTR_WIDTH       = $clog2(PORTS);
    localparam FIFO_WIDTH           = 2 + DATA_WIDTH + 2*ADDR_WIDTH + PORT_PTR_WIDTH;
    localparam RAM_DEPTH            = $rtoi($ceil(VECTOR_LENGTH / NUMBER_OF_RAMS));
    localparam RAM_INDEX_WIDTH      = $clog2(NUMBER_OF_RAMS);
    localparam RAM_ADDRESS_WIDTH    = $clog2(RAM_DEPTH);

    /* verilator lint_off UNUSED */
    logic [PORTS-1:0]               fifo_valid;
    /* verilator lint_on UNUSED */

    logic [PORTS-1:0]               fifo_empty;
    logic [PORTS-1:0]               fifo_full;
    logic                           shift_in;
    logic [PORTS-1:0]               shift_out;

    logic                           write_req;
    logic                           read_req;
    logic [ADDR_WIDTH-1:0]          raddr[PORTS-1:0];
    logic [ADDR_WIDTH-1:0]          waddr[PORTS-1:0];
    logic [DATA_WIDTH-1:0]          wdata[PORTS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      tags[PORTS-1:0];

    logic [RAM_INDEX_WIDTH-1:0]     ram_index[PORTS-1:0];

    logic                           ram_fifo_write_req;
    logic                           ram_fifo_read_req;
    logic [ADDR_WIDTH-1:0]          ram_fifo_raddr[NUMBER_OF_RAMS-1:0];
    logic [ADDR_WIDTH-1:0]          ram_fifo_waddr[NUMBER_OF_RAMS-1:0];
    logic [DATA_WIDTH-1:0]          ram_fifo_wdata[NUMBER_OF_RAMS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      ram_fifo_tags[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_shiftin;
    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_full;

    logic                           ram_write_req;
    logic                           ram_read_req;
    logic [ADDR_WIDTH-1:0]          ram_raddr[NUMBER_OF_RAMS-1:0];
    logic [ADDR_WIDTH-1:0]          ram_waddr[NUMBER_OF_RAMS-1:0];
    logic [DATA_WIDTH-1:0]          ram_wdata[NUMBER_OF_RAMS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      ram_tags[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      ram_valid;

    logic [DATA_WIDTH-1:0]          din[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      rvalids;

    logic [PORTS-1:0]               current_valid;
    logic [DATA_WIDTH-1:0]          dout[PORTS-1:0];

    logic                           flow_ready;
    logic [NUMBER_OF_RAMS-1:0]      ram_shiftout;

    logic [PORTS-1:0]               port_valid_b;
    logic [PORTS-1:0]               port_valid_r;
    logic [PORTS-1:0]               combined_port_valid;

    /* verilator lint_off UNUSED */
    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_empty;
    /* verilator lint_on UNUSED */

    assign shift_in                 = req.wvalid || req.rvalid;
    assign req.arready              = |fifo_full;
    assign req.wready               = |fifo_full;

    genvar i;
    genvar j;

    generate
        for (i = 0; i < PORTS; i++)
        begin : input_fifo_gen

            assign ram_index[i] = (read_req) ? raddr[i][RAM_INDEX_WIDTH-1:0] : waddr[i][RAM_INDEX_WIDTH-1:0];
            assign shift_out[i] = !ram_fifo_full[ram_index[i]] && !fifo_empty[i];

            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
            basic_sync_fifo #(
              .DEPTH            (RAM_FIFO_DEPTH),
              .DATA_WIDTH       (FIFO_WIDTH),
              .READ_LATENCY     (0)
            ) input_fifo_I (
              .clk              (clk),
              .rst_n            (rst_n),

              .din              ({req.rvalid, req.wvalid, req.wdata[i], req.waddr[i], req.raddr[i], PORT_PTR_WIDTH'(i)}),
              .shift_in         (shift_in),

              .full             (fifo_full[i]),

              .dout             ({read_req, write_req, wdata[i], waddr[i], raddr[i], tags[i]}),
              .valid            (fifo_valid[i]),
              .shift_out        (shift_out[i]),
              .empty            (fifo_empty[i])
            );
            /* verilator lint_on PINMISSING */ // (Over/Under)flow Pins

            always_ff @(posedge clk)
            begin : move_to_ram_fifo
                ram_fifo_read_req                   <= read_req;
                ram_fifo_write_req                  <= write_req;
                ram_fifo_wdata[ram_index[i]]        <= wdata[i];
                ram_fifo_waddr[ram_index[i]]        <= waddr[i];
                ram_fifo_raddr[ram_index[i]]        <= raddr[i];
                ram_fifo_tags[ram_index[i]]         <= tags[i];
                ram_fifo_shiftin[ram_index[i]]      <= shift_out[i];
            end
        end
    endgenerate
    
    generate
        for (j = 0; j < NUMBER_OF_RAMS; j++)
        begin : ram_gen
            always_ff @(posedge clk)
            begin
                if (~rst_n)
                begin
                    port_valid_r                <= '0;
                end
                else
                begin
                    port_valid_r[ram_tags[j]]   <= ram_valid[j] && ram_shiftout[j];
                end
            end

            assign ram_shiftout[j]              = !combined_port_valid[ram_tags[j]] && flow_ready;

            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
            basic_sync_fifo #(
              .DEPTH            (4),
              .DATA_WIDTH       (FIFO_WIDTH),
              .READ_LATENCY     (0)
            ) ram_fifo_I  (
              .clk              (clk),
              .rst_n            (rst_n),

              .din              ({ram_fifo_read_req, ram_fifo_write_req, ram_fifo_wdata[j], ram_fifo_waddr[j], ram_fifo_raddr[j], ram_fifo_tags[j]}),
              .shift_in         (ram_fifo_shiftin[j]),

              .full             (ram_fifo_full[j]),

              .dout             ({ram_read_req, ram_write_req, ram_wdata[j], ram_waddr[j], ram_raddr[j], ram_tags[j]}),
              .valid            (ram_valid[j]),
              .shift_out        (ram_shiftout[j]),
              .empty            (ram_fifo_empty[j])
            );
            /* verilator lint_on PINMISSING */ // (Over/Under)flow Pins

            r1w1_ram  #(
              .ADDR_WIDTH   (RAM_ADDRESS_WIDTH),
              .DATA_WIDTH   (DATA_WIDTH),
              .RDELAY       (1)
            ) r1w1_ram_I    (
              .clk          (clk),

              .en           (ram_valid[j]),
              .we           (ram_write_req),
              .addr         ((ram_write_req) ? ram_waddr[j][ADDR_WIDTH-1:RAM_INDEX_WIDTH] : ram_raddr[j][ADDR_WIDTH-1:RAM_INDEX_WIDTH]),
              .wdata        (ram_wdata[j]),

              .data         (din[j]),
              .valid        (rvalids[j])
            );

        end
    endgenerate

    assign combined_port_valid = current_valid | port_valid_r;

    flow_controller #(
        .DATA_WIDTH     (DATA_WIDTH),
        .INPUT_PORTS    (NUMBER_OF_RAMS),
        .OUTPUT_PORTS   (PORTS)
    ) flow_controller_I (
        .clk            (clk),
        .rst_n          (rst_n),

        .din            (din),
        .addr           (ram_tags),
        .valid_in       (rvalids),
        .ready          (flow_ready),

        .dout           (dout),
        .current_valid  (current_valid),
        .valid          (req.rvalid),
        .shift_out      (req.rready)
    );

    always_comb
    begin
        for (int k = 0; k < PORTS; k++)
        begin
            req.rdata[k]    = dout[k];
            req.bdata[k]    = '0;
        end
    end

endmodule
