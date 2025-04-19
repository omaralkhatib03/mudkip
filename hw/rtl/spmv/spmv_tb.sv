`timescale 1ns/1ps

// module spmv_tb #(
//     parameter   VECTOR_LENGTH  = 32,
//     parameter   DATA_WIDTH     = 32,
//     parameter   PARALLELISM    = 4,
//     parameter   RELEASE_MODE   = 0,
//     parameter   FLOAT          = 0,
//     parameter   E_WIDTH        = 8,
//     parameter   FRAC_WIDTH     = 23,
//     parameter   NUMBER_OF_RAMS = 4,
//     parameter   RAM_FIFO_DEPTH = 4,
//     localparam  ADDR_WIDTH     = $clog2(VECTOR_LENGTH)
// ) (
//     input wire                      clk,
//     input wire                      rst_n,

//     input wire                      ping,
//     input wire                      en,
//     input wire                      cfg_en,
//     output logic                    done,

//     input wire [DATA_WIDTH-1:0]     val_data [PARALLELISM-1:0],
//     input wire                      val_valid,
//     output logic                    val_ready,
//     input wire                      val_last,
//     input wire                      val_bytemask,

//     input wire [DATA_WIDTH-1:0]     r_beg_data [PARALLELISM-1:0],
//     input wire                      r_beg_valid,
//     output logic                    r_beg_ready,
//     input wire                      r_beg_last,
//     input wire                      r_beg_bytemask,

//     input wire [DATA_WIDTH-1:0]     c_idx_data [PARALLELISM-1:0],
//     input wire                      c_idx_valid,
//     output logic                    c_idx_ready,
//     input wire                      c_idx_last,
//     input wire                      c_idx_bytemask,

//     input wire [ADDR_WIDTH-1:0]     cfg_addr [PARALLELISM-1:0],
//     input wire [DATA_WIDTH-1:0]     cfg_wdata [PARALLELISM-1:0],
//     input wire                      cfg_write,
//     input wire                      cfg_valid,
//     output logic                    cfg_ready,

//     output logic [DATA_WIDTH-1:0]   cfg_bdata,
//     output logic                    cfg_bvalid,
//     input wire                      cfg_bready,

//     output logic [DATA_WIDTH-1:0]   cfg_rdata [PARALLELISM-1:0],
//     output logic                    cfg_rvalid,
//     input wire                      cfg_rready,

//     input wire [ADDR_WIDTH-1:0]     rom_x_addr [PARALLELISM-1:0],
//     input wire [DATA_WIDTH-1:0]     rom_x_wdata [PARALLELISM-1:0],
//     input wire                      rom_x_write,
//     input wire                      rom_x_valid,
//     output logic                    rom_x_ready,

//     output logic [DATA_WIDTH-1:0]   rom_x_bdata,
//     output logic                    rom_x_bvalid,
//     input wire                      rom_x_bready,

//     output logic [DATA_WIDTH-1:0]   rom_x_rdata [PARALLELISM-1:0],
//     output logic                    rom_x_rvalid,
//     input wire                      rom_x_rready,

//     input wire [ADDR_WIDTH-1:0]     rom_x_n_addr [PARALLELISM-1:0],
//     input wire [DATA_WIDTH-1:0]     rom_x_n_wdata [PARALLELISM-1:0],
//     input wire                      rom_x_n_write,
//     input wire                      rom_x_n_valid,
//     output logic                    rom_x_n_ready,

//     output logic [DATA_WIDTH-1:0]   rom_x_n_bdata,
//     output logic                    rom_x_n_bvalid,
//     input wire                      rom_x_n_bready,

//     output logic [DATA_WIDTH-1:0]   rom_x_n_rdata [PARALLELISM-1:0],
//     output logic                    rom_x_n_rvalid,
//     input wire                      rom_x_n_rready
// );

//     axi_stream_if #(
//         .DATA_WIDTH(DATA_WIDTH),
//         .PARALLELISM(PARALLELISM)
//     ) val_if();

//     axi_stream_if #(
//         .DATA_WIDTH(DATA_WIDTH),
//         .PARALLELISM(PARALLELISM)
//     ) r_beg_if();

//     axi_stream_if #(
//         .DATA_WIDTH(DATA_WIDTH),
//         .PARALLELISM(PARALLELISM)
//     ) c_idx_if();

//     vector_ram_if #(
//         .LENGTH(VECTOR_LENGTH),
//         .DATA_WIDTH(DATA_WIDTH),
//         .PARALLELISM(PARALLELISM),
//         .FLOAT(FLOAT),
//         .E_WIDTH(E_WIDTH),
//         .FRAC_WIDTH(FRAC_WIDTH)
//     ) cfg_if();

//     vector_ram_if #(
//         .LENGTH(VECTOR_LENGTH),
//         .DATA_WIDTH(DATA_WIDTH),
//         .PARALLELISM(PARALLELISM),
//         .FLOAT(FLOAT),
//         .E_WIDTH(E_WIDTH),
//         .FRAC_WIDTH(FRAC_WIDTH)
//     ) rom_x_if();

//     vector_ram_if #(
//         .LENGTH(VECTOR_LENGTH),
//         .DATA_WIDTH(DATA_WIDTH),
//         .PARALLELISM(PARALLELISM),
//         .FLOAT(FLOAT),
//         .E_WIDTH(E_WIDTH),
//         .FRAC_WIDTH(FRAC_WIDTH)
//     ) rom_x_n_if();

//     spmv_kernel_top #(
//         .VECTOR_LENGTH(VECTOR_LENGTH),
//         .DATA_WIDTH(DATA_WIDTH),
//         .PARALLELISM(PARALLELISM),
//         .RELEASE_MODE(RELEASE_MODE),
//         .FLOAT(FLOAT),
//         .E_WIDTH(E_WIDTH),
//         .FRAC_WIDTH(FRAC_WIDTH),
//         .NUMBER_OF_RAMS(NUMBER_OF_RAMS),
//         .RAM_FIFO_DEPTH(RAM_FIFO_DEPTH)
//     ) dut_i (
//         .clk(clk),
//         .rst_n(rst_n),
//         .ping(ping),
//         .en(en),
//         .done(done),

//         .val(val_if),
//         .r_beg(r_beg_if),
//         .c_idx(c_idx_if),

//         .cfg_en(cfg_en),
//         .cfg(cfg_if),

//         .rom_x(rom_x_if),
//         .rom_x_n(rom_x_n_if)
//     );

//     always_comb
//     begin
//         val_if.data         = val_data;
//         val_if.valid        = val_valid;
//         val_ready           = val_if.ready;
//         val_if.last         = val_last;
//         val_if.bytemask     = val_bytemask;

//         r_beg_if.data       = r_beg_data;
//         r_beg_if.valid      = r_beg_valid;
//         r_beg_ready         = r_beg_if.ready;
//         r_beg_if.last       = r_beg_last;
//         r_beg_if.bytemask   = r_beg_bytemask;

//         c_idx_if.data       = c_idx_data;
//         c_idx_if.valid      = c_idx_valid;
//         c_idx_ready         = c_idx_if.ready;
//         c_idx_if.last       = c_idx_last;
//         c_idx_if.bytemask   = c_idx_bytemask;

//         cfg_if.addr         = cfg_addr;
//         cfg_if.wdata        = cfg_wdata;
//         cfg_if.write        = cfg_write;
//         cfg_if.valid        = cfg_valid;
//         cfg_ready           = cfg_if.ready;

//         cfg_bdata           = cfg_if.bdata;
//         cfg_bvalid          = cfg_if.bvalid;
//         cfg_if.bready       = cfg_bready;

//         cfg_rdata           = cfg_if.rdata;
//         cfg_rvalid          = cfg_if.rvalid;
//         cfg_if.rready       = cfg_rready;

//         rom_x_if.addr       = rom_x_addr;
//         rom_x_if.wdata      = rom_x_wdata;
//         rom_x_if.write      = rom_x_write;
//         rom_x_if.valid      = rom_x_valid;
//         rom_x_ready         = rom_x_if.ready;

//         rom_x_bdata         = rom_x_if.bdata;
//         rom_x_bvalid        = rom_x_if.bvalid;
//         rom_x_if.bready     = rom_x_bready;

//         rom_x_rdata         = rom_x_if.rdata;
//         rom_x_rvalid        = rom_x_if.rvalid;
//         rom_x_if.rready     = rom_x_rready;

//         rom_x_n_if.addr     = rom_x_n_addr;
//         rom_x_n_if.wdata    = rom_x_n_wdata;
//         rom_x_n_if.write    = rom_x_n_write;
//         rom_x_n_if.valid    = rom_x_n_valid;
//         rom_x_n_ready       = rom_x_n_if.ready;

//         rom_x_n_bdata       = rom_x_n_if.bdata;
//         rom_x_n_bvalid      = rom_x_n_if.bvalid;
//         rom_x_n_if.bready   = rom_x_n_bready;

//         rom_x_n_rdata       = rom_x_n_if.rdata;
//         rom_x_n_rvalid      = rom_x_n_if.rvalid;
//         rom_x_n_if.rready   = rom_x_n_rready;

//     end

// endmodule
