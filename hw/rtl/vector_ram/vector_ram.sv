`timescale 1ns/1ps

/*
*  Fast Vector Ram should allow for random access to vector data -> Used in SpMV
*
*  This will consume quite a bit of (B/U)RAM for larger problems
*  E.g Assume we are working with single precision numbers (32 bits),
*  a 512 compute bus would require 18 BRAMS in parallel, which is to be
*  increased if we require more memory. I.e does not fit in one BRAM.
*
*  A single BRAM configured for 32 bit width stored 1Kb, which is 512 numbers.
*/

/*
*   Design:
*   If I want to actually utilise the parallelsim function on chip, I need to store vectors efficiently.
*   A vector ram allows for reading and writing vectors into off-chip memory. The main feastue is seperating
*   the parallelism parameter from the number of (B/U)RAMS needed in parallel. For example, you can instantiate
*   a 256 bit bus to process 8 single precision numbers at a time with 4x32 bit RAMS in parallel.
*   This affects throughput ofcourse but we can uses FIFO's to 'maintain' a throughput of one on the input side.
*/

module vector_ram
#(
    parameter PARALLELISM           = 2, // Must be a power of 2, or equal to number of ports
    parameter RAM_FIFO_DEPTH        = 4,
    parameter VECTOR_LENGTH         = 32 // I think this was afiro
) (
    input         wire              clk,
    input         wire              rst_n,
    vector_ram_if.slave             req
);
    localparam PORTS                = req[0].PARALLELISM; // Virtual Ports
    localparam DATA_WIDTH           = req[0].DATA_WIDTH;
    localparam ADDR_WIDTH           = req[0].ADDR_WIDTH;
    localparam PORT_PTR_WIDTH       = $clog2(PORTS);
    localparam NUMBER_OF_RAMS       = PARALLELISM;
    localparam FIFO_WIDTH           = 2 + DATA_WIDTH + 2*ADDR_WIDTH + PORT_PTR_WIDTH;
    localparam RAM_DEPTH            = $rtoi($ceil(VECTOR_LENGTH / PARALLELISM));
    localparam RAM_INDEX_WIDTH      = $clog2(NUMBER_OF_RAMS);
    localparam RAM_ADDRESS_WIDTH    = $clog2(RAM_DEPTH);

    logic [PORTS-1:0]               fifo_valid;
    logic [PORTS-1:0]               fifo_empty;
    logic [PORTS-1:0]               fifo_full;
    logic [PORTS-1:0]               shift_in;

    logic [PORTS-1:0]               write_req;
    logic [PORTS-1:0]               read_req;
    logic [ADDR_WIDTH-1:0]          raddr[PORTS-1:0];
    logic [ADDR_WIDTH-1:0]          waddr[PORTS-1:0];
    logic [DATA_WIDTH-1:0]          wdata[PORTS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      tags[PORTS-1:0];

    logic [RAM_INDEX_WIDTH-1:0]     ram_index[PORTS-1:0];
    logic [PORTS-1:0]               shift_out;

    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_write_req;
    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_read_req;
    logic [ADDR_WIDTH-1:0]          ram_fifo_raddr[NUMBER_OF_RAMS-1:0];
    logic [ADDR_WIDTH-1:0]          ram_fifo_waddr[NUMBER_OF_RAMS-1:0];
    logic [DATA_WIDTH-1:0]          ram_fifo_wdata[NUMBER_OF_RAMS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      ram_fifo_tags[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_shiftin;
    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_full;
    logic [NUMBER_OF_RAMS-1:0]      ram_fifo_empty;

    logic [PORTS-1:0]               ram_write_req;
    logic [PORTS-1:0]               ram_read_req;
    logic [ADDR_WIDTH-1:0]          ram_raddr[NUMBER_OF_RAMS-1:0];
    logic [ADDR_WIDTH-1:0]          ram_waddr[NUMBER_OF_RAMS-1:0];
    logic [DATA_WIDTH-1:0]          ram_wdata[NUMBER_OF_RAMS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      ram_tags[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      ram_shiftout;
    logic [NUMBER_OF_RAMS-1:0]      ram_valid;


    logic [DATA_WIDTH-1:0]          din[NUMBER_OF_RAMS-1:0];
    logic [PORT_PTR_WIDTH-1:0]      port_addr[NUMBER_OF_RAMS-1:0];
    logic [NUMBER_OF_RAMS-1:0]      rvalids;

    logic [PORTS-1:0]               valid_out;
    logic [DATA_WIDTH-1:0]          dout[PORTS-1:0];

    logic [PORTS-1:0]               rready;
    
    generate 
        for (genvar i = 0; i < PORTS; i++)
        begin : shift_in_gen
            assign shift_in[i] = req[i].wvalid || req[i].rvalid;
        end
    endgenerate

    genvar i;
    genvar j;

    generate

        for (i = 0; i < PORTS; i++)
        begin : input_fifo_gen

            assign ram_index[i] = (read_req[i]) ? raddr[i][RAM_INDEX_WIDTH-1:0] : waddr[i][RAM_INDEX_WIDTH-1:0];
            assign shift_out[i] = !ram_fifo_full[ram_index] && fifo_valid[i] && !fifo_empty[i];

            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
            basic_sync_fifo #(
              .DEPTH            (RAM_FIFO_DEPTH),
              .DATA_WIDTH       (FIFO_WIDTH),
              .READ_LATENCY     (0)
            ) vector_ram_fifo_I (
              .clk              (clk),
              .rst_n            (rst_n),

              .din              ({req[i].rvalid, req[i].wvalid, req[i].wdata, req[i].waddr, req[i].raddr, PORT_PTR_WIDTH'(i)}),
              .shift_in         (shift_in[i]),

              .full             (fifo_full[i]),

              .dout             ({read_req[i], write_req[i], wdata[i], waddr[i], raddr[i], tags[i]}),
              .valid            (fifo_valid[i]),
              .shift_out        (shift_out[i]),
              .empty            (fifo_empty[i])
            );
            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins

            always_ff @(posedge clk)
            begin : move_to_ram_fifo
                ram_fifo_read_req[ram_index]    <= read_req[i];
                ram_fifo_write_req[ram_index]   <= write_req[i];
                ram_fifo_wdata[ram_index]       <= wdata[i];
                ram_fifo_waddr[ram_index]       <= waddr[i];
                ram_fifo_raddr[ram_index]       <= raddr[i];
                ram_fifo_tags[ram_index]        <= tags[i];
                ram_fifo_shiftin[ram_index]     <= shift_out[i];
            end

            assign req[i].arready               = fifo_full[i];
            assign req[i].wready                = fifo_full[i];
        end
    endgenerate
    
    logic flow_ready;

    generate
        for (i = 0; i < NUMBER_OF_RAMS; i++)
        begin : ram_gen
            basic_sync_fifo #(
              .DEPTH            (4), // Shallow FIFO For syncing
              .DATA_WIDTH       (FIFO_WIDTH),
              .READ_LATENCY     (0)
            ) b_adapter_fifo_I  (
              .clk              (clk),
              .rst_n            (rst_n),

              .din              ({ram_fifo_read_req[j], ram_fifo_write_req[j], ram_fifo_wdata[j], ram_fifo_waddr[j], ram_fifo_raddr[j], ram_fifo_tags[j]}),
              .shift_in         (ram_fifo_shiftin[j]),

              .full             (ram_fifo_full[j]),

              .dout             ({ram_read_req[j], ram_write_req[j], ram_wdata[j], ram_waddr[j], ram_raddr[j], ram_tags[j]}),
              .valid            (ram_valid[j]),
              .shift_out        (valid_out[ram_tags[j]] && flow_ready),
              .empty            (ram_fifo_empty[j])
            );

            r1w1_ram  #(
              .ADDR_WIDTH   (RAM_ADDRESS_WIDTH),
              .DATA_WIDTH   (DATA_WIDTH),
              .RDELAY       (1)
            ) r1w1_ram_I    (
              .clk          (clk),
              .rst_n        (rst_n),
              .arvalid      (ram_read_req[j] && ram_valid[j]),
              .raddr        (ram_raddr[j][ADDR_WIDTH-1:RAM_INDEX_WIDTH]),
              .rdata        (din[j]),
              .rvalid       (rvalids[j]),
              .wvalid       (ram_write_req[j] && ram_valid[j]),
              .waddr        (ram_waddr[j][ADDR_WIDTH-1:RAM_INDEX_WIDTH]),
              .wdata        (ram_wdata[j])
            );
        end
    endgenerate

    flow_controller #(
        .DATA_WIDTH(DATA_WIDTH),
        .INPUT_PORTS(NUMBER_OF_RAMS),
        .OUTPUT_PORTS(PORTS)
    ) flow_controller_I (
        .clk            (clk),
        .rst_n          (rst_n),

        .din            (din),
        .addr           (port_addr),
        .valid_in       (rvalids),
        .ready          (flow_ready),

        .dout           (dout),
        .valid          (valid_out),
        .front_ready    (|rready)
    );
    
    generate 
        for (genvar k = 0; k < PORTS; k++)
        begin : gen_ouput
            assign req[k].rvalid   = valid_out[k];
            assign req[k].rdata    = dout[k]; 
            assign req[k].arready  = !fifo_full[k];
            assign req[k].wready   = !fifo_full[k];
            assign req[k].bdata    = '0;   
            assign rready[k]       = req[k].rready;
        end
    endgenerate

endmodule
