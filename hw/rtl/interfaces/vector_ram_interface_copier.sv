`timescale 1ns/1ps

module vector_ram_interface_copier #(
) (
    vector_ram_if.slave in,
    vector_ram_if.master out
);

    always_comb
    begin
        out.write    = in.write;
        out.rready   = in.rready;
        out.valid    = in.valid;
        in.ready           = out.ready;
        in.rvalid          = out.rvalid;

        for (int i = 0; i < in.PARALLELISM; i++)
        begin
            in.rdata[i]            = out.rdata[i];
            out.wdata[i]     = in.wdata[i];
            out.addr[i]      = in.addr[i];
        end
    end


endmodule
