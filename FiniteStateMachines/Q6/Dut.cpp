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
        D = 3,
        E = 4,
        F = 5
    };

    State state = A;
    input_t w;
    output_t z;

    while (true) {
#ifndef NATIVE_SYSTEMC
        w = i_x.get();
#else
        w = i_x.read();
#endif

        switch (state) {
            case A:
                state = w ? A : B;
                break;
            case B:
                state = w ? D : C;
                break;
            case C:
                state = w ? D : E;
                break;
            case D:
                state = w ? A : F;
                break;
            case E:
                state = w ? D : E;
                break;
            case F:
                state = w ? D : C;
                break;
        }

        z = (state == E || state == F);

#ifndef NATIVE_SYSTEMC
        o_result.put(z);
#else
        o_result.write(z);
#endif
    }
}
