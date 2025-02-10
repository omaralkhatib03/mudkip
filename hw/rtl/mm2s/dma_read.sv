`timescale 1ns/1ps

module dma_read (

  input clk,
  input rst_n,

  ps_if.slave ps_i,

  // Axi MM 
  axi_lite_if.master mem_i,

  // AXI Stream
  axi_stream_if.master dout_i
);

  /////////////////////////////////////////////////////////////
  ////////////           PS_REGISTERS           ///////////////
  /////////////////////////////////////////////////////////////
  
  typedef enum integer {
    IDLE,
    FETCHING
  } mm2s_state_enum;

  localparam NUMBER_OF_PS_REGISTERS = 7;
  localparam dma_axi_addr = 0, dma_axi_valid = 1, error = 2, length = 3, start = 4, stride = 5, ready = 6;

  logic  [31:0] mem[NUMBER_OF_PS_REGISTERS-1:0]; 
  
  mm2s_state_enum state_b;
  mm2s_state_enum state_r;

  logic [mem_i.ADDR_WIDTH-1:0] mem_addr_r;
  logic [mem_i.ADDR_WIDTH-1:0] mem_addr_b;
  
  logic error_b;
  logic done;

  int dma_length_b;
  int dma_length_r;

  // Write logic   
  always_ff @(posedge clk)
  begin
    if (!rst_n)
    begin
      mem[dma_axi_valid] <= '0;   
    end
    else 
    begin
      if (ps_i.wvalid)
      begin
        case (ps_i.waddr)
          dma_axi_addr:
          begin
              mem[dma_axi_addr]   <= ps_i.wdata; 
              mem[dma_axi_valid]  <= '1; 
          end
          stride: mem[stride]   <= ps_i.wdata; 
          start:  mem[start]    <= ps_i.wdata;
          length: mem[length]   <= ps_i.wdata;
          default: 
          begin
              mem[dma_axi_valid]  <= '0; // Refuse Write if otherwise; 
          end
        endcase   
      end
      else 
      begin
        mem[dma_axi_addr]   <= mem[dma_axi_addr];
        mem[dma_axi_valid]  <= mem[dma_axi_valid];
      end
    end
    ps_i.wresp <= ps_i.wvalid;
    mem[ready] <= 32'(state_r == IDLE);
  end

  // Read logic
  always_ff @(*)
  begin
      if (ps_i.arvalid)
      begin
        case (ps_i.raddr)
          dma_axi_addr: ps_i.rdata    = mem[dma_axi_addr];
          dma_axi_valid: ps_i.rdata   = mem[dma_axi_addr];
          ready: ps_i.rdata           = mem[ready];
          default: ps_i.rdata         = mem[error];
        endcase   
      end
  end

  assign ps_i.wready = '1;
  assign ps_i.rvalid = ps_i.arvalid;

  /////////////////////////////////////////////////////////////
  ////////////           PL_LOGIC: s2mm         ///////////////
  /////////////////////////////////////////////////////////////
  assign done = 32'(mem_addr_r) - mem[dma_axi_addr] == mem[length];
  
  always_comb
  begin
    state_b       = state_r;
    error_b       = 1'(mem[error]);
    mem_addr_b    = mem_addr_r;

    case (state_r)
      IDLE:
      begin
        if (mem[start][0] && mem[dma_axi_valid][0])
        begin
          state_b       = FETCHING; 
          mem_addr_b    = mem_i.ADDR_WIDTH'(mem[dma_axi_valid]); 
        end
        else if (mem[start][0] && !mem[dma_axi_valid][0])
        begin
          error_b = '1;
        end
      end
      FETCHING:
      begin
        if (done)
        begin
          state_b = IDLE; 
        end
        else if (mem_i.arready)
        begin
          mem_addr_b    = mem_addr_r + mem_i.ADDR_WIDTH'(mem[stride]);   
        end
      end
    endcase
  end
  
  always_ff @(posedge clk) 
  begin 
    if (!rst_n)
    begin
      state_r     <= IDLE;
      mem[error]  <= '0;
      mem_addr_r  <= '0;
    end
    else begin
      state_r     <= state_b;
      mem[error]  <= 32'(error_b);
      mem_addr_r  <= mem_addr_b;
    end
  end
  
  assign mem_i.arvalid = state_r == FETCHING && !done;
  assign mem_i.raddr   = mem_addr_r; 

  int read_counter;

  always_ff @(posedge clk) 
  begin 
    if (mem_i.rvalid)
    begin
      read_counter <= read_counter + 1; 
    end
    else if (read_counter == mem[length])
    begin
      read_counter <= 0;
    end
    begin
      read_counter <= read_counter; 
    end
  end

  basic_sync_fifo #(
    .DATA_WIDTH(mem_i.DATA_WIDTH),
    .DEPTH(256),
    .READ_LATENCY(1)
  ) rdata_fifo_I (
    .clk(clk),
    .rst_n(!rst_n),

    .din(mem_i.rdata),
    .shift_in(mem_i.rvalid),

    .shift_out(dout_i.ready),
    .dout(dout_i.data),
    .valid(dout_i.valid),
    
    .full(mem_i.rready),

    .empty(),
    .underflow(),
    .overflow()
  );

  assign dout_i.last = read_counter == mem[length]; 

endmodule

