`timescale 1ns/1ps

/*
* SPMV Reduction Network
*
* This module performs a tree-like reduction on its inputs. Its a specific
* kernel used in SpMV. Namely, its an addition tree where two adjecant
* inputs are added only if they share the same ID. If the to adjecant inputs
* do not share the same ID they're values are propogated through the Network.
* Hence the module has P inputs and P outputs. If two adjecant inputs do share
* the same ID, the resulting sum is placed on the line nearest to the center
* of the Network. Thus if all inputs share the same ID, then the module
* behaves as an adder-tree.
*
* In terms of resources, this module is pretty a** ngl. Each operator requires
* an W bit number comparator and fixed point adder. The number of reduction
* operators required is O(n^2) where n is the width of the Network. (i.e the
* number of inputs). Furthermore, the module only supports an even
* number of inputs. It is possible to extend it to an odd number of inputs but
* (a) i cba, and (b) it slightly reduces performance, and if not that, then
* timing.
*
* (The letters on each lane are contact points (R on left, W on right))
*
* Network Structure:
*
*           a ----------a------------------------- a'
*                   OP
*           b ----------b--------b---------------- b'
*                           OP
*           c ----------c--------c---------------- c'
*                   OP
*           d ----------d------------------------- d'
*
* In theory, the number of inputs should be kept low, and the number of
* partitions the input matrix is split into should be increased. This way the
* reduction network in each kernel is a means of improving bandwidth
* utilisation. Using T processors in paralell, (i.e T partitions) means that
* the resource estimate is O(Tn^2), which is better than just increasing n.
*/

module spmv_reduction_network
#(
    parameter NETWORK_WIDTH                 = 4, // Must be even
    parameter FIFO_DEPTH                    = 2
) (
    input wire clk,
    input wire rst_n,
    network_if.slave    in[NETWORK_WIDTH-1:0],
    network_if.master   out[NETWORK_WIDTH-1:0]
);

    localparam N_2                                      = NETWORK_WIDTH / 2;
    localparam PIPELINE_STAGES                          = 3;

    localparam [PIPELINE_STAGES-1:0][31:0] PIPELINE     = {32'(N_2), 32'd1, 32'd0};

    localparam OUTPUT_WIDTH     = out[0].IN_WIDTH;
    localparam OUTPUT_ID_WIDTH  = out[0].ID_WIDTH;

    network_if #(
        .IN_WIDTH(OUTPUT_WIDTH),
        .ID_WIDTH(OUTPUT_ID_WIDTH)
    ) lanes [(PIPELINE_STAGES * NETWORK_WIDTH) - 1:0] ();

    generate
        for (genvar i = 'd0; i < PIPELINE_STAGES - 1; i++)                            // For all the pipeline stages
        begin : spmv_pipeline_gen

            localparam int LAYERSPERPIPE = PIPELINE[i+1] - PIPELINE[i];

            network_if #(
                .IN_WIDTH(OUTPUT_WIDTH),
                .ID_WIDTH(OUTPUT_ID_WIDTH)
            ) current_lanes [((LAYERSPERPIPE+1)*NETWORK_WIDTH)-1:0] ();

            for (genvar j = 'd0; j < LAYERSPERPIPE; j = j + 1)
            begin : layer_engines_gen
                localparam int iter = j + PIPELINE[i];

                for (genvar k = 0; k < NETWORK_WIDTH - 1; k = k + 1)
                begin : layer_gen
                    if ((k % 2 == iter % 2) && (k >= iter) && (k < NETWORK_WIDTH - iter))
                    begin : op_gen
                        spmv_network_op     #(
                            .LOCATION       (k),
                            .PARALLELISM    (NETWORK_WIDTH)
                        ) spmv_net_op_I     (
                            .in_a           (current_lanes[(j*NETWORK_WIDTH) + k]), // Input Lanes
                            .in_b           (current_lanes[(j*NETWORK_WIDTH) + k + 1]),
                            .out_a          (current_lanes[((j + 1)*NETWORK_WIDTH) + k]),
                            .out_b          (current_lanes[((j + 1)*NETWORK_WIDTH) + k + 1])
                        );
                    end
                    else
                    begin : prop_gen

                        network_if_copier #(
                            .DELAY(0)
                        ) identity_prop_k_I (
                            .clk(clk),
                            .rst_n(rst_n),
                            .in(current_lanes[(j*NETWORK_WIDTH) + k]),
                            .out(current_lanes[((j + 1)*NETWORK_WIDTH) + k])
                        );

                        network_if_copier #(
                            .DELAY(0)
                        ) identity_prop_k_1_I (
                            .clk(clk),
                            .rst_n(rst_n),
                            .in(current_lanes[(j*NETWORK_WIDTH) + k + 1]),
                            .out(current_lanes[((j + 1)*NETWORK_WIDTH) + k + 1])
                        );
                    end
                end
            end

            for (genvar j = 0; j < NETWORK_WIDTH; j++)
            begin : pipeline_prop_gen

                network_if_copier #(
                    .DELAY(0)
                ) pipe_to_curr_copier_I (
                    .clk(clk),
                    .rst_n(rst_n),
                    .in(lanes[(i*NETWORK_WIDTH)+j]),
                    .out(current_lanes[j])
                );

                network_if_copier #(
                    .DELAY(1),
                    .FIFO_DEPTH(FIFO_DEPTH)
                ) pipe_copier_I (
                    .clk(clk),
                    .rst_n(rst_n),
                    .in(current_lanes[((LAYERSPERPIPE) * NETWORK_WIDTH)+j]),
                    .out(lanes[((i+1)*NETWORK_WIDTH)+j])
                );

            end
        end
    endgenerate


    // --------------------------------------------------- //
    // ----------------  Copy IO To Pipe ----------------- //
    // --------------------------------------------------- //

    generate
        for (genvar i = 0; i < NETWORK_WIDTH; i++)
        begin : pipe_copy_gen

            network_if_copier #(.DELAY(1)) pipe_init_copier_I (
                .clk(clk),
                .rst_n(rst_n),
                .in(in[i]),
                .out(lanes[i])
            );

            network_if_copier #(.DELAY(0)) pipe_out_copier_I (
                .clk(clk),
                .rst_n(rst_n),
                .in(lanes[((PIPELINE_STAGES - 1) * NETWORK_WIDTH) + i]),
                .out(out[i])
            );

        end
    endgenerate

endmodule

