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

    enum State {
        S0 = 0,
        S1 = 1,
        S2 = 2,
        S3 = 3,
        S4 = 4
    };

    State state = S0;
    input_t x;
    output_t z;

    while (true) {
#ifndef NATIVE_SYSTEMC
        x = i_x.get();
#else
        x = i_x.read();
#endif

        switch (state) {
            case S0:
                state = x ? S1 : S0;
                break;
            case S1:
                state = x ? S4 : S1;
                break;
            case S2:
                state = x ? S1 : S2;
                break;
            case S3:
                state = x ? S2 : S1;
                break;
            case S4:
                state = x ? S4 : S3;
                break;
        }

        z = (state == S3 || state == S4);

#ifndef NATIVE_SYSTEMC
        o_result.put(z);
#else
        o_result.write(z);
#endif
    }
}
