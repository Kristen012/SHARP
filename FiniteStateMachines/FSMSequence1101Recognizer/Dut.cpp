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
    input_t data;
    output_t start_shifting;

    while (true) {
#ifndef NATIVE_SYSTEMC
        data = i_x.get();
#else
        data = i_x.read();
#endif

        switch (state) {
            case S0:
                state = data ? S1 : S0;
                break;
            case S1:
                state = data ? S2 : S0;
                break;
            case S2:
                state = data ? S2 : S3;
                break;
            case S3:
                state = data ? S4 : S0;
                break;
            case S4:
                state = S4;
                break;
        }

        start_shifting = (state == S4);

#ifndef NATIVE_SYSTEMC
        o_result.put(start_shifting);
#else
        o_result.write(start_shifting);
#endif
    }
}
