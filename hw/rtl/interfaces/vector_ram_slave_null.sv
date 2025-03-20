`timescale 1ns/1ps

module vector_ram_slave_null #()(
    vector_ram_if.slave ram_if
);
    assign ram_if.ready  = 0;
    assign ram_if.bdata  = '{default:0};
    assign ram_if.bvalid = 0;
    assign ram_if.rdata  = '{default:0};
    assign ram_if.rvalid = 0;

endmodule
