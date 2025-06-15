`timescale 1ns/1ps

module s2mm #(
  parameter DATA_WIDTH = 32
) (

  input clk,
  input rst_n,

  ps_if.slave ps_i,
  
  // Axi_s
  axi_stream_if.slave din_i,
  
  // Axi_mm
  axi_lite_if.master dout_i
);

  typedef enum {
    IDLE,
    WRITING
  } s2mm_state_enum;
  
  s2mm_state_enum state_b;
  s2mm_state_enum state_r;

  /////////////////////////////////////////////////////////////
  ////////////           PS_REGISTERS           ///////////////
  /////////////////////////////////////////////////////////////
  
  localparam NUMBER_OF_PS_REGISTERS = 3;
  localparam dma_axi_addr = 0, dma_axi_valid = 1, error = 2, length = 3;

  logic  [31:0] mem[NUMBER_OF_PS_REGISTERS-1:0]; 

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
  end

  // Read logic
  always_ff @(*)
  begin
      if (ps_i.arvalid)
      begin
        case (ps_i.raddr)
          dma_axi_addr: ps_i.rdata   = mem[dma_axi_addr];
          dma_axi_valid: ps_i.rdata  = mem[dma_axi_addr];
          default: ps_i.rdata        = mem[error];
        endcase   
      end
  end

  assign ps_i.wready = '1;
  assign ps_i.rvalid = ps_i.arvalid;

  /////////////////////////////////////////////////////////////
  ////////////           PL_LOGIC: s2mm         ///////////////
  /////////////////////////////////////////////////////////////
  
  localparam ADDR_WIDTH = dout_i.ADDR_WIDTH;

  logic [ADDR_WIDTH-1:0] ram_addr_b;
  logic [ADDR_WIDTH-1:0] ram_addr_r;
  logic error_b;
  logic [DATA_WIDTH-1:0] length_b;

  always_comb
  begin
    state_b     = state_r;
    ram_addr_b  = ram_addr_r;
    error_b     = 1'(mem[error]);
    length_b    = mem[length];

    case (state_r)
      default: 
      begin
        if (din_i.valid)
        begin
          state_b = WRITING; 
          ram_addr_b = ADDR_WIDTH'(mem[dma_axi_addr]);
          error_b = !1'(mem[dma_axi_valid]); 
        end
      end
      WRITING:
      begin
        if (din_i.last)
        begin
          state_b = IDLE; 
          length_b = DATA_WIDTH'(ram_addr_r) - mem[dma_axi_addr];
        end
        else 
        begin
          ram_addr_b = ram_addr_r + ADDR_WIDTH'(DATA_WIDTH / 8);// Single Word Increment  
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
      mem[length] <= '0;
      ram_addr_r  <= '0;
    end
    else 
    begin
      state_r     <= state_b; 
      mem[error]  <= DATA_WIDTH'(error_b);
      mem[length] <= length_b;
      ram_addr_r  <= ram_addr_b;
    end
  end

endmodule
