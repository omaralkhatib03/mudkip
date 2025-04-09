`timescale 1ns / 1ps

module spmv_network_op #(
    parameter LOCATION      = 3,
    parameter PARALLELISM   = 50
) (
    network_if.slave    in,
    network_if.master   out
);
    
    /*
    * If this TOWARDS_CENTER is 1, then the output is on A otherwise the output is
    * on B. 
    * To illustrate the idea, the network is laid out as follows 
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

    localparam TOWARDS_CENTER = LOCATION > $ceil(PARALLELISM / 2); 

    logic calc;
    
    assign calc = in.a.valid && in.b.valid && out.ready;

    always_comb
    begin
        out.b.id    = in.b.id;             
        out.a.id    = in.a.id;             

        if ((in.a.id == in.b.id) && calc)
        begin

            out.a.val   = in.a.val + in.b.val; 

            if (TOWARDS_CENTER)
            begin
                out.a.valid = in.a.valid;
                out.b.valid = '0;
            end
            else
            begin
                out.b.valid = in.b.valid;
                out.b.val   = out.a.val; 
                out.a.valid = '0; 
            end
        end
        else 
        begin
            out.a.val   = out.IN_WIDTH'(in.a.val);

            out.b.val   = out.IN_WIDTH'(in.b.val);

            out.a.valid = in.a.valid;
            out.b.valid = in.b.valid;
        end

        in.ready    = out.ready;
    end

endmodule

