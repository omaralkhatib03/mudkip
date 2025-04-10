`timescale 1ns / 1ps

module spmv_network_op #(
    parameter LOCATION      = 3,
    parameter PARALLELISM   = 50
) (
    network_if.slave    in_a,
    network_if.slave    in_b,

    network_if.master   out_a,
    network_if.master   out_b
);
    
    /*
    * If this TOWARDS_CENTER is 1, then the output is on A otherwise the output is
    * on B. 
    * To illustrate the idea, the network is laid out_as follows 
    *
    *               a       a       a 
    *               b       b       b 
    *               a       a       a   
    *               b       b       b 
    *               a       a       a
    *
    * We always want to accumulate towards the center, hence if the two ids
    * are the same we will add them and output the value towards the center 
    * of the network.
    */

    localparam TOWARDS_CENTER = LOCATION >= $ceil(PARALLELISM / 2); 

    logic calc;
    
    assign calc = in_a.valid && in_b.valid && out_a.ready && out_b.ready;

    always_comb
    begin
        out_b.id    = in_b.id;             
        out_a.id    = in_a.id;             

        if ((in_a.id == in_b.id) && calc)
        begin

            out_a.val   = in_a.val + in_b.val; 

            if (TOWARDS_CENTER)
            begin
                out_a.valid = in_a.valid;
                out_b.valid = '0;
            end
            else
            begin
                out_b.valid = in_b.valid;
                out_b.val   = out_a.val; 
                out_a.valid = '0; 
            end
        end
        else 
        begin
            out_a.val   = out_a.IN_WIDTH'(in_a.val);

            out_b.val   = out_b.IN_WIDTH'(in_b.val);

            out_a.valid = in_a.valid;
            out_b.valid = in_b.valid;
        end

        in_a.ready    = out_a.ready && out_b.ready;
        in_b.ready    = in_a.ready;
    end

endmodule

