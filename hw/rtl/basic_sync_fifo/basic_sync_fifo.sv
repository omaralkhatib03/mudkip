`uselib lib = xpm
`timescale 1 ps/1 ps

module basic_sync_fifo #(
  parameter DATA_WIDTH        = 32,
  parameter DEPTH             = 16,
  parameter READ_LATENCY      = 0,
  localparam DATA_COUNT_WDITH = $clog2(DEPTH)
) (
  input wire                    clk,
  input wire                    rst_n,

  input wire [DATA_WIDTH-1:0]   din,
  input wire                    shift_in,
  
  input wire                    shift_out,
  output logic [DATA_WIDTH-1:0] dout,
  output logic                  valid,

  output logic                  full,
  output logic                  empty,
  output logic                  overflow,
  output logic                  underflow 
  
);

  logic valid_inter; 

  generate
  if (READ_LATENCY == 0)
  begin
    assign valid = !empty; 
  end
  else 
  begin
    assign valid = valid_inter;   
  end
  endgenerate

  xpm_fifo_sync #(
      .FIFO_MEMORY_TYPE         ("auto"), // String
      .FIFO_READ_LATENCY        (1),     // DECIMAL
      .FIFO_WRITE_DEPTH         (DEPTH),   // DECIMAL
      .READ_DATA_WIDTH          (DATA_WIDTH),      // DECIMAL
      .WRITE_DATA_WIDTH         (DATA_WIDTH),     // DECIMAL
      .RD_DATA_COUNT_WIDTH      (DATA_COUNT_WDITH),   // DECIMAL
      .READ_MODE                ("std"),         // String
      .WR_DATA_COUNT_WIDTH      (DATA_COUNT_WDITH),   // DECIMAL
      .FULL_RESET_VALUE         (0),
      .WAKEUP_TIME              (0),
      .USE_ADV_FEATURES         ("1101")
   ) xpm_fifo_sync_inst (
      .dout                 (dout),                  
      .empty                (empty),                
      .full                 (full),                  
      .overflow             (overflow),          
      .rd_data_count        (),
      .wr_data_count        (),
      .underflow            (underflow),        
      .din                  (din),                    
      .rd_en                (shift_out),                
      .rst                  (!rst_n),                    
      .wr_clk               (clk),             
      .wr_en                (shift_in),
      .data_valid           (valid_inter)
   );    

  
endmodule;

