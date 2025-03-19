`timescale 1ns/1ps

module vector_ram_master_null #() (
    vector_ram_if.master ram_if
);
    assign ram_if.addr   = '{default:0};
    assign ram_if.wdata  = '{default:0};
    assign ram_if.write  = '{default:0};
    assign ram_if.valid  = '{default:0};
    assign ram_if.bready = '{default:0};
    assign ram_if.rready = '{default:0};
endmodule
