#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_data.clk_rst(i_clk, i_rst);
	i_done_counting.clk_rst(i_clk, i_rst);
	i_ack.clk_rst(i_clk, i_rst);
    o_shift_ena.clk_rst(i_clk, i_rst);
    o_counting.clk_rst(i_clk, i_rst);
    o_done.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_data.reset();
		i_done_counting.reset();
		i_ack.reset();
		o_shift_ena.reset();
		o_counting.reset();
		o_done.reset();
#endif
		wait();
	}

    enum State {
        S = 0,
        S1 = 1,
        S11 = 2,
        S110 = 3,
        B0 = 4,
        B1 = 5,
        B2 = 6,
        B3 = 7,
        COUNT = 8,
        WAIT = 9
    };

    State state = S;
    input_t data, done_counting, ack;
    output_t shift_ena, counting, done;

    while (true) {
#ifndef NATIVE_SYSTEMC
        data = i_data.get();
        done_counting = i_done_counting.get();
        ack = i_ack.get();
#else
        data = i_data.read();
        done_counting = i_done_counting.read();
        ack = i_ack.read();
#endif

        switch (state) {
            case S:
                state = data ? S1 : S;
                break;
            case S1:
                state = data ? S11 : S;
                break;
            case S11:
                state = data ? S11 : S110;
                break;
            case S110:
                state = data ? B0 : S;
                break;
            case B0:
                state = B1;
                break;
            case B1:
                state = B2;
                break;
            case B2:
                state = B3;
                break;
            case B3:
                state = COUNT;
                break;
            case COUNT:
                state = done_counting ? WAIT : COUNT;
                break;
            case WAIT:
                state = ack ? S : WAIT;
                break;
        }

        shift_ena = (state == B0 || state == B1 || state == B2 || state == B3);
        counting = (state == COUNT);
        done = (state == WAIT);

#ifndef NATIVE_SYSTEMC
        o_shift_ena.put(shift_ena);
        o_counting.put(counting);
        o_done.put(done);
#else
        o_shift_ena.write(shift_ena);
        o_counting.write(counting);
        o_done.write(done);
#endif
    }
}
