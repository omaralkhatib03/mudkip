`timescale 1ns/1ps

module funnel 
(
  input wire clk,
  input wire rst_n,

  axi_stream_if.slave funnel_in,

  axi_stream_if.master funnel_out
);
  
  localparam CYCLES = funnel_in.DATA_WIDTH / funnel_out.DATA_WIDTH;

  logic empty;
  logic shift_out;

  logic [funnel_in.DATA_WIDTH-1:0] fifo_data;
  logic fifo_last;
  logic fifo_valid;

  assign shift_out = !empty && funnel_out.ready;
 
  basic_sync_fifo #(
    .DEPTH(128),
    .DATA_WIDTH(funnel_in.DATA_WIDTH + 1), // for last signal
    .READ_LATENCY(1)
  ) in_fifo_I ( 
    .clk(clk),
    .rst_n(rst_n),
    
    .shift_in(funnel_in.valid),
    .din({funnel_in.data, funnel_in.last}),
    
    .shift_out(shift_out),
    .dout ({fifo_data, fifo_last}),
    .valid(fifo_valid),

    .empty(empty),

    .full(),
    .underflow(),
    .overflow()

  );
  
  typedef enum integer {IDLE, FEEDING} funnel_state_enum;
  
  funnel_state_enum state_b;
  funnel_state_enum state_r;

  logic [$clog2(CYCLES)-1:0] cntr_r;
  logic [$clog2(CYCLES)-1:0] cntr_b;

  logic [funnel_out.DATA_WIDTH-1:0] data_out_r;
  logic [funnel_out.DATA_WIDTH-1:0] data_out_b;

  logic out_last_r;
  logic out_last_b;

  always_comb
  begin
    state_b     = state_r;    
    cntr_b      = cntr_r;
    data_out_b  = data_out_r; 
    out_last_b  = out_last_r;  

    case (state_r)
      IDLE: 
      begin
        if (fifo_valid)
        begin
          state_b = FEEDING;
          data_out_b = fifo_data;          
        end
      end
      FEEDING:
      begin
        if (funnel_out.ready)
        begin

          cntr_b      = cntr_r + 1;
          data_out_b  = fifo_data >> funnel_in.DATA_WIDTH; 

          if (32'(cntr_r) == CYCLES)
          begin
            state_b     = IDLE;
            out_last_r  = fifo_last;
          end   
        end

      end
    endcase
  end
  
  always_ff @(posedge clk) 
  begin     
    if (!rst_n)
    begin
      state_r <= IDLE; 
      cntr_r  <= 0;
    end
    else
    begin
      state_b <= state_b; 
      cntr_r  <= cntr_b;
    end
  end
  
  assign funnel_out.valid = state_r == FEEDING; 
  assign funnel_out.data  = data_out_r; 
  assign funnel_out.last  = out_last_r;

endmodule
