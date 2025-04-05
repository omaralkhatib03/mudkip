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
*
*   Design:
*   If I want to actually utilise the parallelsim function on chip, I need to store vectors efficiently.
*   A vector ram allows for reading and writing vectors into off-chip memory. The main feature is seperating
*   the parallelism parameter from the number of (B/U)RAMS needed in parallel. For example, you can instantiate
*   a 256 bit bus to process 8 single precision numbers at a time with 4x32 bit RAMS in parallel.
*   This affects throughput of course but we can uses FIFO's to virtually 'maintain' a throughput of one on the input side.
*/

// TODO: Currently this is a functional model, working on a synthesiable model in mudkip/vector-ram-dev

module vector_ram #(
    //verilator lint_off unused
    parameter NUMBER_OF_RAMS = 2,
    parameter RAM_FIFO_DEPTH = 4
    //verilator lint_on unused
)(
    input         wire              clk,
    // verilator lint_off unused
    input         wire              rst_n,
    // verilator lint_on unused
    vector_ram_if.slave             req,
    vector_ram_if.slave             rom
);

    localparam PORTS                = req.PARALLELISM; // Virtual Ports
    localparam DATA_WIDTH           = req.DATA_WIDTH;
    localparam LENGTH               = req.LENGTH;

    logic [DATA_WIDTH-1:0] mem [LENGTH-1:0];

    always_ff @(posedge clk)
    begin
        for (int j = 0; j < PORTS; j++)
        begin
            if (req.write && req.valid)
            begin
                mem[req.addr[j]] <= req.wdata[j];
            end
            req.rdata[j]    <= mem[req.addr[j]];
            rom.rdata[j]    <= mem[rom.addr[j]];
        end

        req.rvalid      <= req.valid && !req.write;
        rom.rvalid      <= rom.valid && !rom.write;
    end

    assign req.ready       = 1'b1;
    assign rom.ready       = 1'b1;

endmodule
