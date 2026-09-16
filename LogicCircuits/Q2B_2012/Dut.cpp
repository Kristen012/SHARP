#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_y.clk_rst(i_clk, i_rst);
	i_w.clk_rst(i_clk, i_rst);
    o_Y1.clk_rst(i_clk, i_rst);
    o_Y3.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_y.reset();
		i_w.reset();
		o_Y1.reset();
		o_Y3.reset();
#endif
		wait();
	}

    input_y_t y;
    input_t w;
    output_t Y1, Y3;

    while (true) {
#ifndef NATIVE_SYSTEMC
        y = i_y.get();
        w = i_w.get();
#else
        y = i_y.read();
        w = i_w.read();
#endif

		Y1 = w & y[0];
		Y3 = ~w & (y[1] | y[2] | y[4] | y[5]);

#ifndef NATIVE_SYSTEMC
        o_Y1.put(Y1);
        o_Y3.put(Y3);
#else
        o_Y1.write(Y1);
        o_Y3.write(Y3);
#endif
    }
}
