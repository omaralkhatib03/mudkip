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
    localparam FIFO_WIDTH           = 1 + DATA_WIDTH + ADDR_WIDTH + PORT_PTR_WIDTH;
    localparam RAM_DEPTH            = $rtoi($ceil(VECTOR_LENGTH / NUMBER_OF_RAMS));
    localparam RAM_INDEX_WIDTH      = $clog2(NUMBER_OF_RAMS);
    localparam RAM_ADDRESS_WIDTH    = $clog2(RAM_DEPTH);

    logic [RAM_INDEX_WIDTH-1:0]     ram_index[PORTS-1:0];
    logic                           ram_write_req;
    logic [ADDR_WIDTH-1:0]          ram_addr[NUMBER_OF_RAMS-1:0];
    logic [DATA_WIDTH-1:0]          ram_wdata[NUMBER_OF_RAMS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      ram_tags[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      ram_valid;
    logic [DATA_WIDTH-1:0]          din[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      rvalids;
    logic [PORTS-1:0]               current_valid;
    logic [DATA_WIDTH-1:0]          dout[PORTS-1:0];
    logic                           flow_ready;
    logic [NUMBER_OF_RAMS-1:0]      rr_ready;
    logic [PORTS-1:0]               port_valid_r;
    logic [PORTS-1:0]               combined_port_valid;
    logic [PORTS-1:0]               rr_req[NUMBER_OF_RAMS-1:0];
    logic [FIFO_WIDTH-1:0]          rr_din[PORTS-1:0]; 
    logic [PORTS-1:0] in_ready;

    genvar j;

    assign req.ready = &in_ready;

    always_comb 
    begin

        for (int i = 0; i < PORTS; i++)
        begin : input_fifo_gen
            ram_index[i]            = req.addr[i][RAM_INDEX_WIDTH-1:0];
            rr_din[i]               = {req.write, req.wdata[i], PORT_PTR_WIDTH'(i), req.addr[i]};
            for (int k = 0; k < NUMBER_OF_RAMS; k++)
            begin
                rr_req[k][i]        = (ram_index[i] == RAM_INDEX_WIDTH'(k));
            end
        end
    end

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
                    port_valid_r[ram_tags[j]]   <= ram_valid[j] && rr_ready[j];
                end
            end

            assign rr_ready[j]                  = !combined_port_valid[ram_tags[j]] && flow_ready;

            so_rr_arbiter #(
                .NUM_INPUTS(PORTS),
                .DATA_WIDTH(FIFO_WIDTH),
                .FIFO_DEPTH(RAM_FIFO_DEPTH)
            ) rr_arbiters_I (
                .clk        (clk),
                .rst_n      (rst_n),
            
                .req        (rr_req[j]),
                .in         (rr_din),
                .in_ready   (in_ready),
            
                .valid      (ram_valid[j]),
                .dout       ({ram_write_req, ram_wdata[j], ram_tags[j], ram_addr[j]}),
                .ready      (rr_ready[j]) 
            );

            r1w1_ram  #(
              .ADDR_WIDTH   (RAM_ADDRESS_WIDTH),
              .DATA_WIDTH   (DATA_WIDTH),
              .RDELAY       (1)
            ) r1w1_ram_I    (
              .clk          (clk),

              .en           (ram_valid[j]),
              .we           (ram_write_req),
              .addr         (ram_addr[j][ADDR_WIDTH-1:RAM_INDEX_WIDTH]),
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
