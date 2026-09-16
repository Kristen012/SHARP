#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_y.clk_rst(i_clk, i_rst);
	i_w.clk_rst(i_clk, i_rst);
    o_Y2.clk_rst(i_clk, i_rst);
    o_Y4.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_y.reset();
		i_w.reset();
		o_Y2.reset();
		o_Y4.reset();
#endif
		wait();
	}

    input_y_t y;
    input_t w;
    output_t Y2, Y4;

    while (true) {
#ifndef NATIVE_SYSTEMC
        y = i_y.get();
        w = i_w.get();
#else
        y = i_y.read();
        w = i_w.read();
#endif

		Y2 = y[0] & ~w;
		Y4 = (y[1] | y[2] | y[4] | y[5]) & w;

#ifndef NATIVE_SYSTEMC
        o_Y2.put(Y2);
        o_Y4.put(Y4);
#else
        o_Y2.write(Y2);
        o_Y4.write(Y4);
#endif
    }
}
