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
        A = 0,
        B = 1,
        C = 2,
        D = 3
    };

    State state = A;

    while (true) {
        input_t in;
#ifndef NATIVE_SYSTEMC
        in = i_x.get();
#else
        in = i_x.read();
#endif

        switch (state) {
            case A:
                state = (in == 0) ? A : B;
                break;
            case B:
                state = (in == 0) ? C : B;
                break;
            case C:
                state = (in == 0) ? A : D;
                break;
            case D:
                state = (in == 0) ? C : B;
                break;
        }

        output_t out;
        out = (state == D) ? 1 : 0;

#ifndef NATIVE_SYSTEMC
        o_result.put(out);
#else
        o_result.write(out);
#endif
    }
}
