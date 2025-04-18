`timescale 1ns / 1ps



module uram_tb #(
    parameter AWIDTH = 12,  // Address Width
    parameter DWIDTH = 32,  // Data Width
    parameter NBPIPE = 1    // Number of pipeline Registers
) ( 

    input wire clk, 

    // Port A
    input wea,                    // Write Enable
    input mem_ena,                // Memory Enable
    input [DWIDTH-1:0] dina,      // Data Input  
    input [AWIDTH-1:0] addra,     // Address Input
    output reg [DWIDTH-1:0] douta,// Data Output

    // Port B
    input web,                    // Write Enable
    input mem_enb,                // Memory Enable
    input [DWIDTH-1:0] dinb,      // Data Input  
    input [AWIDTH-1:0] addrb,     // Address Input
    output reg [DWIDTH-1:0] doutb // Data Output

);
    

    generate
        for (genvar i = 0; i < 55; i++)
        begin : gen_uram_blocks
             uram # (
                .AWIDTH(AWIDTH),
                .DWIDTH(DWIDTH),
                .NBPIPE(NBPIPE)
            ) dut_I (
                .clk(clk),   
                .wea(wea),    
                .mem_ena(mem_ena),
                .dina(dina), 
                .addra(addra),

                .douta(douta),
                .web(web),    
                .mem_enb(mem_enb),
                .dinb(dinb), 
                .addrb(addrb),
                .doutb(doutb)
            );
        end
    endgenerate

endmodule

