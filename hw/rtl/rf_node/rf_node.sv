`timescale 1ns/1ps

module rf_node (
  input clk,
  input rst_n,

  ps_if.slave ps_i
);

  localparam NUMBER_OF_PS_REGISTERS = 2**ps_i.ADDR_WIDTH;
  localparam dma_axi_addr = 0, dma_axi_valid = 1;

  logic  [31:0] mem[NUMBER_OF_PS_REGISTERS-1:0]; 
  logic  [31:0] mem_b[NUMBER_OF_PS_REGISTERS-1:0]; 
  
  always_comb 
  begin 
    for (int i = 0; i < NUMBER_OF_PS_REGISTERS; i++) 
    begin
      mem_b[i] = mem[i];
    end

    if (ps_i.wvalid)
    begin
      mem_b[ps_i.waddr] = ps_i.wdata;
    end

  end

  // Write logic   
  always_ff @(posedge clk)
  begin
    if (!rst_n)
    begin
      mem[dma_axi_valid] <= '0; // Reset important registers, not all of them
    end
    else 
    begin
      mem <= mem_b;
      ps_i.bvalid <= ps_i.wvalid;
    end
  end

  assign ps_i.wready  = '1;
  assign ps_i.aready  = '1;
  assign ps_i.rvalid  = ps_i.arvalid;
  assign ps_i.rdata   = ps_i.arvalid ? mem[ps_i.raddr] : '0;

endmodule;

