`timescale 1ns / 1ps

package spmv_pkg;

    typedef enum integer {
        IDLE,       // Wait for enable
        BUSY        // Busy computing
    } spmv_kernel_state_enum;


endpackage : spmv_pkg
