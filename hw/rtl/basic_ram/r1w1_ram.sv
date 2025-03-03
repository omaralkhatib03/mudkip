`timescale 1 ns/1 ps

module r1w1_ram
#(
  parameter ADDR_WIDTH = 32,
  parameter DATA_WIDTH = 32,
  parameter RDELAY = 0
) (
  input wire                    clk,
  input wire                    en,
  input wire                    we,
  input wire [ADDR_WIDTH-1:0]   addr,
  input wire [DATA_WIDTH-1:0]   wdata,

  output logic [DATA_WIDTH-1:0] data,
  output logic                  valid

);
    localparam MEMS = 1 << ADDR_WIDTH;

    logic [DATA_WIDTH-1:0] mem [MEMS-1:0];

    always_ff @(posedge clk)
    begin
        if (we && en)
        begin
            mem[addr] <= wdata;
        end
    end

    generate
        if (RDELAY == 0)
        begin : delay_0_ram
            assign data    = mem[addr];
            assign valid   = en && !we;
        end
        else
        begin : delay_1_ram
           always_ff @(posedge clk)
           begin : delay_1_ram_reg
                data   <= mem[addr];
                valid  <= en && !we;
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



