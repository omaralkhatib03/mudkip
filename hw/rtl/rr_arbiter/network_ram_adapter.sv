`timescale 1 ns / 100 ps


module network_ram_adapter #(
    // Network Width --> Can make this big
    parameter NUM_INPUTS    = 32,

    // Vector Length --> Then make this small
    parameter NUM_OUTPUTS   =  4,
    parameter DATA_WIDTH    = 32,
    parameter ID_WIDTH      = $clog2(NUM_OUTPUTS),
    parameter FIFO_DEPTH    = 16
) (
    input wire                      clk, 
    input wire                      rst_n,

    input wire [DATA_WIDTH-1:0]     in_val [NUM_INPUTS-1:0],
    input wire [ID_WIDTH-1:0]       in_id [NUM_INPUTS-1:0], // The output port targetted
    input wire [NUM_INPUTS-1:0]     in_valid, 
    output logic [NUM_INPUTS-1:0]   in_ready,
    
    // The ith NUM will write to the ith id (Hence the ID is no longer needed,
    // can be reobtained via the order of num_outputs)
    output logic [DATA_WIDTH-1:0]   val [NUM_OUTPUTS-1:0],
    output logic [ID_WIDTH-1:0]     id [NUM_OUTPUTS-1:0], // The output port targetted
    output logic [NUM_OUTPUTS-1:0]  valid, 
    input wire [NUM_OUTPUTS-1:0]    ready
);
    
    // verilator lint_off unused    
    logic [NUM_INPUTS-1:0] fifo_empty;
    // verilator lint_on unused    
    
    logic [NUM_INPUTS-1:0] fifo_valid;
    logic [NUM_INPUTS-1:0] fifo_full;

    logic [DATA_WIDTH-1:0] fifo_out_val[NUM_INPUTS-1:0];
    logic [ID_WIDTH-1:0] fifo_out_id[NUM_INPUTS-1:0];

    generate 
        for (genvar i = 0; i < NUM_INPUTS; i++)
        begin : fifo_gen

            // verilator lint_off PINMISSING
            basic_sync_fifo #(
                .DATA_WIDTH     (ID_WIDTH + DATA_WIDTH),
                .DEPTH          (FIFO_DEPTH),
                .READ_LATENCY   (1)
            ) input_fifo_I      (
                .clk            (clk),
                .rst_n          (rst_n),
                .din            ({in_id[i], in_val[i]}),
                .shift_in       (in_valid[i]),
                .shift_out      (ready[in_id[i]]),
                .valid          (fifo_valid[i]),
                .dout           ({fifo_out_id[i], fifo_out_val[i]}),
                .full           (fifo_full[i]),
                .empty          (fifo_empty[i])
            );            
            // verilator lint_off PINMISSING
            
            assign in_ready[i] = !fifo_full[i]; 

        end
    endgenerate
    

    always_comb 
    begin 
        valid = '0; 
        
        for (int i = 0; i < NUM_INPUTS; i++)
        begin
            valid[fifo_out_id[i]]   = fifo_valid[i];
            id[fifo_out_id[i]]      = fifo_out_id[i];
            val[fifo_out_id[i]]     = fifo_out_val[i];
        end

    end

endmodule

