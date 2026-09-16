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
        IDLE = 0,
        START = 1,
        S1 = 2,
        S2 = 3,
        S3 = 4,
        S4 = 5,
        S5 = 6,
        S6 = 7,
        S7 = 8,
        S8 = 9,
        STOP = 10,
        WAIT = 11
    };

    State state = IDLE;

    while (true) {
        input_t in;
#ifndef NATIVE_SYSTEMC
        in = i_x.get();
#else
        in = i_x.read();
#endif

        switch (state) {
            case IDLE:
                state = in ? IDLE : START;
                break;
            case START:
                state = S1;
                break;
            case S1:
                state = S2;
                break;
            case S2:
                state = S3;
                break;
            case S3:
                state = S4;
                break;
            case S4:
                state = S5;
                break;
            case S5:
                state = S6;
                break;
            case S6:
                state = S7;
                break;
            case S7:
                state = S8;
                break;
            case S8:
                state = in ? STOP : WAIT;
                break;
            case STOP:
                state = in ? IDLE : START;
                break;
            case WAIT:
                state = in ? IDLE : WAIT;
                break;
        }

        output_t out;
        out = (state == STOP);

#ifndef NATIVE_SYSTEMC
        o_result.put(out);
#else
        o_result.write(out);
#endif
    }
}
