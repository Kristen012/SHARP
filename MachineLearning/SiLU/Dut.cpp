#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		o_result.reset();
#endif
		wait();
	}
    const input_t one = input_t(1.0f);

    while (true) {
#ifndef NATIVE_SYSTEMC
        input_x = i_x.get();
#else
        input_x = i_x.read();
#endif
        input_t ans = input_x / (one + exp(-input_x));
#ifndef NATIVE_SYSTEMC
        o_result.put(ans);
#else
        o_result.write(ans);
#endif
    }
}
