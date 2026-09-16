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
    input_t r;
    output_t g;

    while (true) {
#ifndef NATIVE_SYSTEMC
        r = i_x.get();
#else
        r = i_x.read();
#endif

        switch (state) {
            case A:
                if (r[0]) {
                    state = B;
                }
                else if (~r[0] & r[1]) {
                    state = C;
                }
                else if (~r[0] & ~r[1] & r[2]) {
                    state = D;
                }
                else {
                    state = A;
                }
                break;
            case B:
                if (r[0]) {
                    state = B;
                }
                else {
                    state = A;
                }
                break;
            case C:
                if (r[1]) {
                    state = C;
                }
                else {
                    state = A;
                }
                break;
            case D:
                if (r[2]) {
                    state = D;
                }
                else {
                    state = A;
                }
                break;
        }

        if (state == A) {
            g = 0;
        }
        else if (state == B) {
            g = 1;
        }
        else if (state == C) {
            g = 2;
        }
        else if (state == D) {
            g = 4;
        }

#ifndef NATIVE_SYSTEMC
        o_result.put(g);
#else
        o_result.write(g);
#endif
    }
}
