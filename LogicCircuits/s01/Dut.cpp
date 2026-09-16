#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
    i_x1.clk_rst(i_clk, i_rst);
    i_x2.clk_rst(i_clk, i_rst);
    i_x3.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif
}

void Dut::do_compute() {
    {
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
        i_x1.reset();
        i_x2.reset();
        i_x3.reset();
        o_result.reset();
#endif
        wait();
    }

    while (true) {
        input_t a, b, c;
        output_t result;

#ifndef NATIVE_SYSTEMC
        a = i_x1.get();
        b = i_x2.get();
        c = i_x3.get();
#else
        a = i_x1.read();
        b = i_x2.read();
        c = i_x3.read();
#endif

        // Logic equivalent to the Verilog always block
        if (a && b && c) 
            result = 1;
        else if (a && b && !c) 
            result = 0;
        else if (a && !b && c) 
            result = 1;
        else if (!a && b && c) 
            result = 0;
        else if (a && !b && !c) 
            result = 1;
        else if (!a && !b && c) 
            result = 0;
        else if (!a && b && !c) 
            result = 1;
        else if (!a && !b && !c) 
            result = 0;
        else
            result = 0;

#ifndef NATIVE_SYSTEMC
        o_result.put(result);
#else
        o_result.write(result);
#endif
    }
}
