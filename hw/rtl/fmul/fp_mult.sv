`timescale 1ns/1ps
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//File Name: Multiplication.v
//Created By: Sheetal Swaroop Burada
//Date: 30-04-2019
//Project Name: Design of 32 Bit Floating Point ALU Based on Standard IEEE-754 in Verilog and its implementation on FPGA.
//University: Dayalbagh Educational Institute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

module fp_mult #(
  parameter BIT_SIZE  = 32,
  parameter EXPONENT  = 8,
  parameter MANITSSA  = 23
) (
	input wire [BIT_SIZE-1:0]   a_operand,
	input wire [BIT_SIZE-1:0]   b_operand,
	output wire         Exception,Overflow,Underflow,
	output wire [BIT_SIZE-1:0]  result
);

localparam IMPLICIT_MANTISSA  = MANITSSA - 1;
localparam SIGN_BIT           = BIT_SIZE-1;

wire sign,product_round,normalised,zero,norm_sign;
wire [EXPONENT-1:0] exponent,sum_exponent;
wire [IMPLICIT_MANTISSA:0] product_mantissa;
wire [MANITSSA:0] operand_a,operand_b;
wire [2*MANITSSA + 1:0] product,product_normalised; //48 Bits


assign sign = a_operand[SIGN_BIT] ^ b_operand[SIGN_BIT];

//Exception flag sets 1 if either one of the exponent is 255.
assign Exception = (&a_operand[BIT_SIZE-2:MANITSSA]) | (&b_operand[BIT_SIZE-2:MANITSSA]);

//Assigining significand values according to Hidden Bit.
//If exponent is equal to zero then hidden bit will be 0 for that respective significand else it will be 1

assign operand_a = (|a_operand[BIT_SIZE-2:MANITSSA]) ? {1'b1,a_operand[IMPLICIT_MANTISSA:0]} : {1'b0,a_operand[IMPLICIT_MANTISSA:0]};

assign operand_b = (|b_operand[BIT_SIZE-2:MANITSSA]) ? {1'b1,b_operand[IMPLICIT_MANTISSA:0]} : {1'b0,b_operand[IMPLICIT_MANTISSA:0]};

assign product = operand_a * operand_b;			//Calculating Product

assign product_round = |product_normalised[IMPLICIT_MANTISSA:0];  //Ending 22 bits are OR'ed for rounding operation.

assign normalised = product[2 * MANITSSA + 1] ? 1'b1 : 1'b0;

assign product_normalised = normalised ? product : product << 1;	//Assigning Normalised value based on 48th bit

//Final Manitssa.
assign norm_sign = (product_normalised[MANITSSA] & product_round);

assign product_mantissa = product_normalised[2*MANITSSA:MANITSSA+1] + MANITSSA'(norm_sign);

assign zero = Exception ? 1'b0 : (product_mantissa == '0) ? 1'b1 : 1'b0;

assign sum_exponent = a_operand[BIT_SIZE-2:MANITSSA] + b_operand[BIT_SIZE-2:MANITSSA];

assign exponent = sum_exponent - (1 << (BIT_SIZE-1)) + EXPONENT'(normalised);

assign Overflow = ((exponent[EXPONENT] & !exponent[EXPONENT-1]) & !zero) ; //If overall exponent is greater than 255 then Overflow condition.
//Exception Case when exponent reaches its maximu value that is 384.

//If sum of both exponents is less than 127 then Underflow condition.
assign Underflow = ((exponent[EXPONENT] & exponent[EXPONENT-1]) & !zero) ? 1'b1 : 1'b0;

assign result = Exception ? '0 :
                zero ? {sign,(BIT_SIZE-1)'(0)} :
                Overflow ? {sign,{EXPONENT{1'b1}},(MANITSSA)'(0)} :
                Underflow ? {sign,(BIT_SIZE-1)'(0)} :
                {sign,exponent[EXPONENT-1:0],product_mantissa};

endmodule
