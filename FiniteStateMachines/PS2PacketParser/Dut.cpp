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
        S1 = 0,
        S2 = 1,
        S3 = 2,
        DONE = 3
    };

    State state = S1;

    while (true) {
        input_t in;
#ifndef NATIVE_SYSTEMC
        in = i_x.get();
#else
        in = i_x.read();
#endif

        output_t out;
        out = (state == DONE);

        switch (state) {
            case S1:
                state = in[3] ? S2 : S1;
                break;
            case S2:
                state = S3;
                break;
            case S3:
                state = DONE;
                break;
            case DONE:
                state = in[3] ? S2 : S1;
                break;
        }

#ifndef NATIVE_SYSTEMC
        o_result.put(out);
#else
        o_result.write(out);
#endif
    }
}
