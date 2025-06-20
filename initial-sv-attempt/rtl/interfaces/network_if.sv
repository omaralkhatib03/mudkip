`timescale 1ns / 1ps

interface network_if;

    parameter IN_WIDTH      = 32;
    parameter ID_WIDTH      = 32;

    logic [ID_WIDTH-1:0]    id;
    logic [IN_WIDTH-1:0]    val;

    logic                   ready;
    logic                   valid;

    modport slave(
        input  id,
        input  val,
        input  valid,
        output ready
    );

    modport master(
        output  id,
        output  val,
        output  valid,
        input   ready
    );

endinterface

