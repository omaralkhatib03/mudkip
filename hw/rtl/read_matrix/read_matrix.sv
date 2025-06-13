
module strip_padding #(
    parameter DATA_WIDTH    = 32,
    parameter ELEMENTS      = 9
) (
    input wire              clk,
    input wire              rst_n,

    input wire              last,

    axi_stream_if.slave     in,
    axi_stream_if.master    out
);

    typedef enum integer {
        IDLE,
        STREAM,
        WAIT
    } state_enum ;

    state_enum state_b;
    state_enum state_r;
    

    always_comb 
    begin
        state_b = state_r;

        case (state_r)
        IDLE:
        begin
            
        end
        STREAM:
        begin
            
        end
        WAIT:
        begin
            
        end
        endcase
    end
    

    always_ff @(posedge clk) 
    begin
        if (!rst_n)
        begin
            state_r <= IDLE;      
        end
        else 
        begin
            state_b <= state_b; 
        end
    end

    // assign outputs
    always_comb 
    begin 
        
    end

endmodule


module align_matrix #(

) (
    axi_stream_if.slave                 r_beg_ddr,
    axi_stream_if.slave                 c_idx_ddr,
    axi_stream_if.slave                 c_val_ddr,

    output logic [c_val.DATA_WIDTH-1:0] c_idx [EL_PER_DDR - 1 :0],
    output logic [c_idx.DATA_WIDTH-1:0] val [EL_PER_DDR - 1 :0],
    output logic [r_beg.DATA_WIDTH-1:0] r_idx[EL_PER_DDR - 1 :0],
    output logic                        valid,
    input wire                          ready,
    output logic [EL_PER_DDR-1:0]       mask,
    output logic                        last
);


endmodule

module read_matrix #(
    parameter EL_PER_DDR = 16,
    parameter OFFSET = 0
) (
    input wire              clk,
    input wire              rst_n,

    axi_stream_if.slave     r_beg_ddr,
    axi_stream_if.slave     c_idx_ddr,
    axi_stream_if.slave     c_val_ddr,

    output logic [EL_PER_DDR - 1 :0][c_val.DATA_WIDTH-1:0] c_idx,
    output logic [EL_PER_DDR - 1 :0][c_idx.DATA_WIDTH-1:0] val,
    output logic [EL_PER_DDR - 1 :0][r_beg.DATA_WIDTH-1:0] r_idx,
    output logic valid,
    input wire ready,
    output logic [EL_PER_DDR-1:0] mask,
    output logic last

);

    localparam ROW_PTR_WIDTH = r_beg_ddr.DATA_WIDTH;

    logic last_inter;

    axi_stream_if #(
        .DATA_WIDTH(ROW_PTR_WIDTH),
        .PARALLELISM(EL_PER_DDR)
    ) r_idx_padded_if();

    axi_stream_if #(
        .DATA_WIDTH(ROW_PTR_WIDTH),
        .PARALLELISM(EL_PER_DDR)
    ) r_idx_if();

    row_decoder #(
        .OFFSET(OFFSET),
    ) row_decoder_I (
        .clk        (clk),
        .rst_n      (rst_n),
        .r_beg      (r_beg_ddr),
        .last       (last_inter)
        .row_ids    (r_idx_padded_if)
    );

    strip_padding #(
        .DATA_WIDTH (ROW_PTR_WIDTH),
        .ELEMENTS   (EL_PER_DDR)
    ) strip_padding_I (
        .clk(clk),
        .rst_n(rst_n),
        .last(last_inter),
        .in(r_idx_padded_if),
        .out(r_idx_if)
    );

    align_matrix aligner_I(
        .clk       (clk),
        .rst_n     (rst_n)
        .r_idx_s   (r_idx_if),
        .c_idx_s   (c_idx_ddr),
        .c_val_s   (c_val),

        .c_idx     (c_idx),
        .val       (val),
        .r_idx     (r_idx),
        .valid     (valid),
        .ready     (ready),
        .mask      (mask),
        .last      (last)
    );

endmodule
