`timescale 1ns / 1ps

module network_if_copier #( 
    parameter DELAY = 1
) (
    // verilator lint_off unused
    input logic clk,
    // veirlator lint_on unused
    //
    network_if.slave  in,
    network_if.master out
);

    generate
        if (DELAY == 0) 
        begin : comb_gen 
            // verilator lint_off width 
            assign out.val      = in.val;
            assign out.id       = in.id;
            // verilator lint_on width 
            assign out.valid    = in.valid;
            assign in.ready     = out.ready;
        end 
        else 
        begin : delay_gen
            always_ff @(posedge clk) begin
                // verilator lint_off width 
                out.val         <= in.val;
                out.id          <= in.id;
                // verilator lint_on width 
                out.valid       <= in.valid;
                in.ready        <= out.ready;
            end
        end
    endgenerate


endmodule
