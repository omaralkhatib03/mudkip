`timescale 1ns/1ps

module fixed_acc #(
    parameter ACC_WIDTH = 128,
    parameter ACC_FRAC  = 64,
    parameter IN_WIDTH  = 32, 
    parameter IN_FRAC   = 16
) (
    input wire                      clk, 
    input wire                      rst_n,
    
    input wire [IN_WIDTH-1:0]       a, 
    input wire                      nd,           // acts like nd
    input wire                      done,            // reset acc 

    output logic [ACC_WIDTH-1:0]    acc,
    output logic                    valid 
);

    localparam SHIFT_VAL = ACC_FRAC - IN_FRAC;

    always_ff @(posedge clk) 
    begin 
        if (!rst_n)
        begin
            valid   <= '0;
            acc     <= '0; 
        end
        else if (done)
        begin
            valid   <= '0;
            acc     <= '0; 
        end
        else if (nd)
        begin
            if (SHIFT_VAL > 0)
            begin
                acc <= acc + (ACC_WIDTH'(a) << SHIFT_VAL);
            end 
            else 
            begin
                acc <= acc + (ACC_WIDTH'(a) >> -SHIFT_VAL);
            end
            valid <= 1;
        end
    end

endmodule
