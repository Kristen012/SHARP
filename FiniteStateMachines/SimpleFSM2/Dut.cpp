#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_j.clk_rst(i_clk, i_rst);
	i_k.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_j.reset();
		i_k.reset();
		o_result.reset();
#endif
		wait();
	}

    enum State {
        OFF = 0,
        ON = 1
    };

    State state = OFF;

    while (true) {
        input_t j, k;
#ifndef NATIVE_SYSTEMC
        j = i_j.get();
        k = i_k.get();
#else
        j = i_j.read();
        k = i_k.read();
#endif

        switch (state) {
            case ON:
                state = (k == 0) ? ON : OFF;
                break;
            case OFF:
                state = (j == 0) ? OFF : ON;
                break;
        }

        output_t out;
        out = (state == OFF) ? 0 : 1;

#ifndef NATIVE_SYSTEMC
        o_result.put(out);
#else
        o_result.write(out);
#endif
    }
}
