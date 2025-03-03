`timescale 1 ns/1 ps

package assert_pkg;

    // Macro to assert equality between x and y
    `define ASSERT_EQUAL(x, y) \
        assert (x == y) else \
            $fatal("Assertion failed: x (%0d) is not equal to y (%0d)", x, y);

endpackage
