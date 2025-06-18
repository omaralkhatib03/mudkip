`timescale 1ns/1ps

// TODO: Add Option to Pipeline 
module oh_to_binary  
#(
  parameter INPUT_WIDTH = 3,
  localparam OUT_WIDTH = $clog2(INPUT_WIDTH)
) (
  input wire [INPUT_WIDTH-1:0] in,
  output logic [OUT_WIDTH-1:0] out
);

  always_comb
  begin
    out = '0;

    for (int i = 0; i < INPUT_WIDTH; i++)
    begin
      if (in[i] == '1)
      begin
        out = OUT_WIDTH'(i);
      end
    end
  end

endmodule
