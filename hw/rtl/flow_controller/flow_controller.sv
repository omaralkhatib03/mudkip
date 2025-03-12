`timescale 1ns/1ps

module flow_controller #(
    parameter   DATA_WIDTH    = 32,
    parameter   INPUT_PORTS   = 3,
    parameter   OUTPUT_PORTS  = 3,
    localparam  PORT_POINTER = $clog2(OUTPUT_PORTS)
) (
    input wire                      clk,
    input wire                      rst_n,

    input wire [DATA_WIDTH-1:0]     din[INPUT_PORTS-1:0],
    input wire [PORT_POINTER-1:0]   addr[INPUT_PORTS-1:0],
    input wire [INPUT_PORTS-1:0]    valid_in,
    output logic                    ready,

    output logic [DATA_WIDTH-1:0]   dout[OUTPUT_PORTS-1:0],
    output logic [OUTPUT_PORTS-1:0] current_valid,
    output logic                    valid,
    input wire                      shift_out 
);

    logic [OUTPUT_PORTS-1:0]        current_valid_b;
    logic [DATA_WIDTH-1:0]          dout_b[OUTPUT_PORTS-1:0];
    logic                           valid_b;
    
    assign valid_b                          = &current_valid;

    always_comb
    begin

        for (int i = 0; i < OUTPUT_PORTS; i++)
        begin
            dout_b[i]                       = dout[i];
        end

        current_valid_b                     = current_valid;

        for (int i = 0; i < INPUT_PORTS; i++)
        begin
            if (valid_in[i] && ready)
            begin
                dout_b[addr[i]]             = din[i];      
                current_valid_b[addr[i]]    = '1;
            end
        end
    end

    always_ff @(posedge clk) 
    begin
        if (!rst_n)
        begin
            valid           <= '0;
            current_valid   <= '0;
        end
        else if (shift_out)
        begin
            valid           <= '0;
            current_valid   <= '0;
        end
        else 
        begin
            valid           <= valid_b;
            current_valid   <= current_valid_b;
        end
    end

    always_ff @(posedge clk)
    begin
        for (int i = 0; i < OUTPUT_PORTS; i++)
        begin
            dout[i]         <= dout_b[i];
        end
    end

    assign ready            = shift_out || !valid;

endmodule
