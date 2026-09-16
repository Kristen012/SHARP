#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
	i_y.clk_rst(i_clk, i_rst);
	i_resetn.clk_rst(i_clk, i_rst);
    o_f.clk_rst(i_clk, i_rst);
    o_g.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		i_y.reset();
		i_resetn.reset();
		o_f.reset();
		o_g.reset();
#endif
		wait();
	}

    enum State {
        IDLE = 0,
        S1 = 1,
        S2 = 2,
        S3 = 3,
        S4 = 4,
        S5 = 5,
        FOREVER_ONE = 6,
        FOREVER_ZERO = 7,
        F_OUT = 8
    };

    State state = IDLE;
    input_t x, y, resetn;
    output_t f, g;

    while (true) {
#ifndef NATIVE_SYSTEMC
        x = i_x.get();
        y = i_y.get();
        resetn = i_resetn.get();
#else
        x = i_x.read();
        y = i_y.read();
        resetn = i_resetn.read();
#endif

        switch (state) {
            case IDLE:
                state = F_OUT;
                break;
            case F_OUT:
                state = S1;
                break;
            case S1:
                state = x ? S2 : S1;
                break;
            case S2:
                state = x ? S2 : S3;
                break;
            case S3:
                state = x ? S4 : S1;
                break;
            case S4:
                state = y ? FOREVER_ONE : S5;
                break;
            case S5:
                state = y ? FOREVER_ONE : FOREVER_ZERO;
                break;
            case FOREVER_ONE:
                state = FOREVER_ONE;
                break;
            case FOREVER_ZERO:
                state = FOREVER_ZERO;
                break;
        }

        f = (state == F_OUT);
        g = (state == S4 || state == S5 || state == FOREVER_ONE);


        if (!resetn) {
            state = IDLE;
            f = 0;
            g = 0;
        }

#ifndef NATIVE_SYSTEMC
        o_f.put(f);
        o_g.put(g);
#else
        o_f.write(f);
        o_g.write(g);
#endif
    }
}
