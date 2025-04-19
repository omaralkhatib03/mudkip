`timescale 1ns/1ps

/* The rows come in encoded using row offsets, we want to decode them such
*  that we get r_{i+1} - r{i} of i for all i. Hence we get a stream of row ids
*  instead of the offsets.
*/

/* Possible Optimisaiton:
* Coalescing between row ids
*/

module row_decoder #(
    parameter OFFSET = 0
) (
    input wire              clk,
    input wire              rst_n,

    axi_stream_if.slave     r_beg,
    axi_stream_if.master    row_ids
);

    localparam IN_PAR       = r_beg.PARALLELISM;
    localparam INTERNAL_PAR = IN_PAR + 1;
    localparam OUT_PAR      = row_ids.PARALLELISM;
    // localparam IN_TOTAL_WIDTH = r_beg.TOTAL_WIDTH;

    localparam ID_WIDTH     = row_ids.DATA_WIDTH;
    localparam OUT_PAR_WIDTH = $clog2(OUT_PAR);

    axi_stream_if #( .DATA_WIDTH(ID_WIDTH), .PARALLELISM(IN_PAR))   r_beg_fifo_if();

    axi_stream_if #( .DATA_WIDTH(ID_WIDTH), .PARALLELISM(INTERNAL_PAR))   r_beg_p0_b();
    axi_stream_if #( .DATA_WIDTH(ID_WIDTH), .PARALLELISM(INTERNAL_PAR))   r_beg_p0_r();

    axi_stream_if #( .DATA_WIDTH(ID_WIDTH), .PARALLELISM(INTERNAL_PAR))   r_beg_p1_b();
    axi_stream_if #( .DATA_WIDTH(ID_WIDTH), .PARALLELISM(INTERNAL_PAR))   r_beg_p1_r();

    axi_stream_if #( .DATA_WIDTH(ID_WIDTH), .PARALLELISM(OUT_PAR))  row_ids_b();

    // ----------------------------------------- //
    // --------------   FIFO   ----------------- //
    // ----------------------------------------- //

    axi_stream_fifo #(.PIPELINE_ONLY(1), .SKID(1)) input_fifo_I (
        .clk        (clk),
        .rst_n      (rst_n),
        .in         (r_beg),
        .out        (r_beg_fifo_if)
    );

    // ----------------------------------------- //
    // -------------- Controller  -------------- //
    // ----------------------------------------- //

    typedef enum int { IDLE, BUSY } state_t; // idle is for shift out, ready is for counting down
    typedef enum int { BIAS_0, PREV_BIAS } bias_state_t; // idle is for shift out, ready is for counting down

    state_t state_b, state_r;
    bias_state_t bias_state_b, bias_state_r;

    logic [ID_WIDTH-1:0] diff_id;
    logic [ID_WIDTH-1:0] number_of_ids_b;
    logic [ID_WIDTH-1:0] number_of_ids_r;

    logic [ID_WIDTH-1:0] last_offset_b;
    logic [ID_WIDTH-1:0] last_offset_r;

    logic [INTERNAL_PAR-1:0][ID_WIDTH-1:0] curr_ids_b;
    logic [INTERNAL_PAR-1:0][ID_WIDTH-1:0] curr_ids_r;

    logic [OUT_PAR_WIDTH-1:0] current_mask_b;
    logic [OUT_PAR_WIDTH-1:0] current_mask_r;

    logic current_last_b;
    logic current_last_r;

    logic [ID_WIDTH-1:0] offset_b;
    logic [ID_WIDTH-1:0] offset_r;

    logic [ID_WIDTH-1:0] outputs_so_far_b;
    logic [ID_WIDTH-1:0] outputs_so_far_r;

    logic [ID_WIDTH-1:0] total_ids_in_chunk_b;
    logic [ID_WIDTH-1:0] total_ids_in_chunk_r;

    always_comb
    begin
        state_b                 = state_r;
        curr_ids_b              = curr_ids_r;
        current_last_b          = current_last_r;
        current_mask_b          = current_mask_r;
        last_offset_b           = last_offset_r;
        r_beg_p0_b.last         = 0;
        total_ids_in_chunk_b    = total_ids_in_chunk_r;
        number_of_ids_b         = number_of_ids_r;

        case (state_r)
            IDLE:
            begin

                last_offset_b = bias_state_r == BIAS_0 ? r_beg_fifo_if.data[0] : last_offset_r;
                offset_b      = bias_state_r == BIAS_0 ? 0 : (offset_r + IN_PAR);

                if (r_beg_fifo_if.valid && r_beg_fifo_if.ready) // Pop the Data on the thing now
                begin
                    state_b         = BUSY;
                    bias_state_b    = PREV_BIAS;
                    current_last_b  = r_beg_fifo_if.last;

                    if (r_beg_fifo_if.last)
                    begin
                        for (int i = 0; i < IN_PAR; i++)
                            if (r_beg_fifo_if.mask[i]) begin
                                curr_ids_b[i+1]     = r_beg_fifo_if.data[i];
                                diff_id             = r_beg_fifo_if.data[i] - last_offset_b;
                            end else begin
                                curr_ids_b[i+1]     = {ID_WIDTH{1'b1}};
                            end
                            curr_ids_b[0]           = last_offset_b;
                     end
                    else
                    begin
                        curr_ids_b          = {r_beg_fifo_if.data, last_offset_b}; // Might pipeline this
                        diff_id             = r_beg_fifo_if.data[IN_PAR-1] - last_offset_b;
                    end

                    number_of_ids_b         = diff_id >> OUT_PAR_WIDTH;
                    current_mask_b          = diff_id[OUT_PAR_WIDTH-1:0];
                    total_ids_in_chunk_b    = diff_id;
                end
            end
            BUSY:
            begin
                if (r_beg_p0_b.ready)
                begin
                    if (number_of_ids_r == 0 && current_last_r)
                    begin
                        bias_state_b    = BIAS_0;
                        state_b         = IDLE;
                        last_offset_b   = r_beg_fifo_if.data[0];
                        r_beg_p0_b.last = 1;
                    end
                    else if (number_of_ids_r == 0)
                    begin
                        state_b         = IDLE;
                        last_offset_b   = curr_ids_r[IN_PAR];
                        r_beg_p0_b.last = 1;
                    end
                    else
                    begin
                        number_of_ids_b = number_of_ids_r - 1;
                    end
                end
            end
        endcase
    end

    assign r_beg_fifo_if.ready  = (state_r == IDLE) && r_beg_p0_b.ready;
    assign r_beg_p0_b.valid     = (state_r == BUSY);
    assign r_beg_p0_b.data      = curr_ids_r;
    assign r_beg_p0_b.mask      = (1 << current_mask_r) - 1;
    assign outputs_so_far_b     = total_ids_in_chunk_r - ((number_of_ids_r << OUT_PAR_WIDTH) + ID_WIDTH'(current_mask_r));

    always_ff @(posedge clk)
    begin
        if (!rst_n)
        begin
            state_r                 <= IDLE;
            bias_state_r            <= BIAS_0;
            current_last_r          <= 0;
        end
        else
        begin
            state_r                 <= state_b;
            last_offset_r           <= last_offset_b;
            curr_ids_r              <= curr_ids_b;
            number_of_ids_r         <= number_of_ids_b;
            bias_state_r            <= bias_state_b;
            current_mask_r          <= current_mask_b;
            current_last_r          <= current_last_b;
            total_ids_in_chunk_r    <= total_ids_in_chunk_b;
        end
    end

    pipeline #(
        .DATA_WIDTH(INTERNAL_PAR*ID_WIDTH + 1 + INTERNAL_PAR + (ID_WIDTH) + ID_WIDTH),
        .PIPE_LINE(1)
    ) pipeline_0_I (
        .clk        (clk),
        .rst_n      (rst_n),
        .in_data    ({r_beg_p0_b.data, r_beg_p0_b.last, r_beg_p0_b.mask, offset_b, outputs_so_far_b}),
        .in_valid   (r_beg_p0_b.valid),
        .in_ready   (r_beg_p0_b.ready),
        .out_data   ({r_beg_p0_r.data, r_beg_p0_r.last, r_beg_p0_r.mask, offset_r, outputs_so_far_r}),
        .out_valid  (r_beg_p0_r.valid),
        .out_ready  (r_beg_p0_r.ready)
    );

    // ----------------------------------------- //
    // --------------   PIPE0  ----------------- //
    // ----------------------------------------- //


    logic [IN_PAR:0][ID_WIDTH-1:0]      subs;
    logic [IN_PAR:0][ID_WIDTH-1:0]      subs_r;

    logic [IN_PAR:0]      nw_comp_b;
    logic [IN_PAR:0]      nw_comp_r;

    logic [ID_WIDTH-1:0]    offset_p0_r;

    // Obtain active region and subs
    always_comb
    begin
        nw_comp_b           = nw_comp_r;
        subs                = subs_r;

        if (r_beg_p0_r.valid && r_beg_p0_r.ready)
        begin
            for (int i = 0; i < INTERNAL_PAR; i++)
            begin
                subs[i]         = (r_beg_p0_r.data[i] - r_beg_p0_r.data[0] - outputs_so_far_r);
                nw_comp_b[i]    = (subs[i] < OUT_PAR);
            end
        end
    end

    // ----------------------------------------- //
    // --------------   PIPE1  ----------------- //
    // ----------------------------------------- //

    pipeline #(
        .DATA_WIDTH(INTERNAL_PAR * ID_WIDTH + 1 + 2*INTERNAL_PAR + ID_WIDTH),
        .PIPE_LINE(1)
    ) pipeline_1_I (
        .clk        (clk),
        .rst_n      (rst_n),
        .in_data    ({subs, r_beg_p0_r.last, r_beg_p0_r.mask, nw_comp_b, offset_r}),
        .in_valid   (r_beg_p0_r.valid),
        .in_ready   (r_beg_p0_r.ready),
        .out_data   ({subs_r, r_beg_p1_r.last, r_beg_p1_r.mask, nw_comp_r, offset_p0_r}),
        .out_valid  (r_beg_p1_r.valid),
        .out_ready  (r_beg_p1_r.ready)
    );

    // compute payload
    logic [OUT_PAR-1:0][ID_WIDTH-1:0] payload_b;
    logic [OUT_PAR-1:0][ID_WIDTH-1:0] payload_r;
    logic [INTERNAL_PAR-1:0][OUT_PAR_WIDTH-1:0] index;
    
    // timing go ahh loop 
    always_comb
    begin

        index = 0;
        payload_b = row_ids.data;

        if (r_beg_p1_r.valid && r_beg_p1_r.ready)
        begin

            for (int i = 0; i < OUT_PAR; i++)
            begin
                payload_b[i] = row_ids.data[IN_PAR-1];
            end

            for (int i = 0; i < INTERNAL_PAR; i++)
            begin
                if (nw_comp_r[i])
                begin
                    index[i] = OUT_PAR_WIDTH'(subs_r[i]);

                    for (int j = 0; j < OUT_PAR; j++)
                    begin
                        if (j >= index[i])
                        begin
                            payload_b[j] = offset_p0_r + OFFSET + i - 1;
                        end
                    end
                end
            end
        end
    end

    assign payload_r               = payload_b;
    assign row_ids_b.valid         = r_beg_p1_r.valid; // I have stuff to process
    assign row_ids_b.last          = r_beg_p1_r.last;
    assign row_ids_b.mask          = OUT_PAR'(r_beg_p1_r.mask);

    assign row_ids_b.data   = payload_r;
    assign r_beg_p1_r.ready = row_ids_b.ready;

    // ----------------------------------------- //
    // --------------  PIPE OUT  --------------- //
    // ----------------------------------------- //

    axi_stream_fifo #( .PIPELINE_ONLY(1), .SKID(1) ) pipe_out_I (
        .clk        (clk),
        .rst_n      (rst_n),
        .in         (row_ids_b),
        .out        (row_ids)
    );

endmodule

