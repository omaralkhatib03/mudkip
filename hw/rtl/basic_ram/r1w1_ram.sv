`timescale 1 ns/1 ps

module r1w1_ram 
#(
  parameter ADDR_WIDTH = 32,
  parameter DATA_WIDTH = 32,
  parameter RDELAY = 0
) (
  input wire                    clk,
  input wire                    rst_n,

  input wire                    arvalid,
  input wire [ADDR_WIDTH-1:0]   raddr,

  output logic [DATA_WIDTH-1:0] rdata,
  output logic                  rvalid,

  input wire                    wvalid,
  input wire [ADDR_WIDTH-1:0]   waddr,
  input wire [DATA_WIDTH-1:0]   wdata

);
    localparam MEMS = 1 << ADDR_WIDTH;

    logic [DATA_WIDTH-1:0] mem_r [MEMS-1:0];
    logic [DATA_WIDTH-1:0] mem_b [MEMS-1:0];

    always_comb
    begin
        mem_b = mem_r;

        if (wvalid)
        begin
            mem_b[waddr] = wdata;
        end
    end

    always_ff @(posedge clk)
    begin
        if (!rst_n)
        begin
            for (int i = 0; i < MEMS; i++)
            begin
                mem_r[i] <= '0;
            end
        end
        else
        begin
            mem_r <= mem_b;
        end
    end
    
    generate
        if (RDELAY == 0)
        begin : delay_0_ram
            assign rdata    = mem_r[raddr];
            assign rvalid   = arvalid; 
        end
        else 
        begin : delay_1_ram
           always_ff @(posedge clk) 
           begin : delay_1_ram_reg
                rdata   <= mem_r[raddr]; 
                rvalid  <= arvalid;
           end 
        end
    endgenerate

endmodule

// r1w1_ram  #(
//   .ADDR_WIDTH(),
//   .DATA_WIDTH(),
//   .RDELAY()
// ) r1w1_ram_I (
//   .clk(),
//   .rst_n(),
//   .arvalid(),
//   .raddr(),
//   .rdata(),
//   .rvalid(),
//   .wvalid(),
//   .waddr(),
//   .wdata()
// );


