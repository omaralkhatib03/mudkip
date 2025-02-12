`timescale 1ns/1ps

module rf_node (

  input clk,
  input rst_n,

  ps_if.slave ps_i
);

  localparam NUMBER_OF_PS_REGISTERS = 3;
  localparam dma_axi_addr = 0, dma_axi_valid = 1, error = 2;

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

endmodule;

