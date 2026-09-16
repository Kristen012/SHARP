#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x1.clk_rst(i_clk, i_rst);
    i_x2.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x1.reset();
		i_x2.reset();
		o_result.reset();
#endif
		wait();
	}

    while (true) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            line_x1[i] = i_x1.get();
            line_x2[i] = i_x2.get();
#else
            line_x1[i] = i_x1.read();
            line_x2[i] = i_x2.read();
#endif
        }

        for (int i = 0; i < ARRAY_SIZE; i++) {
            result[i] = line_x1[i] + line_x2[i];
        }

        for (int i = 0; i < ARRAY_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            o_result.put(result[i]);
#else
            o_result.write(result[i]);
#endif
        }
    }
}
