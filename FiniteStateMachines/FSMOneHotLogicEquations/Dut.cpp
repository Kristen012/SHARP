#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_d.clk_rst(i_clk, i_rst);
	i_done_counting.clk_rst(i_clk, i_rst);
	i_ack.clk_rst(i_clk, i_rst);
	i_state.clk_rst(i_clk, i_rst);
    o_B3_next.clk_rst(i_clk, i_rst);
    o_S_next.clk_rst(i_clk, i_rst);
    o_S1_next.clk_rst(i_clk, i_rst);
    o_Count_next.clk_rst(i_clk, i_rst);
    o_Wait_next.clk_rst(i_clk, i_rst);
    o_done.clk_rst(i_clk, i_rst);
    o_counting.clk_rst(i_clk, i_rst);
    o_shift_ena.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_d.reset();
		i_done_counting.reset();
		i_ack.reset();
		i_state.reset();
		o_B3_next.reset();
		o_S_next.reset();
		o_S1_next.reset();
		o_Count_next.reset();
		o_Wait_next.reset();
		o_done.reset();
		o_counting.reset();
		o_shift_ena.reset();
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

    input_t d, done_counting, ack;
    input_state_t state;
    output_t B3_next, S_next, S1_next, Count_next, Wait_next, done, counting, shift_ena;

    while (true) {
#ifndef NATIVE_SYSTEMC
        d = i_d.get();
        done_counting = i_done_counting.get();
        ack = i_ack.get();
        state = i_state.get();
#else
        d = i_d.read();
        done_counting = i_done_counting.read();
        ack = i_ack.read();
        state = i_state.read();
#endif

        B3_next = state[B2];
        S_next = ~d & state[S] | ~d & state[S1] | ~d & state[S110] | ack & state[WAIT];
        S1_next = d & state[S];
        Count_next = state[B3] | ~done_counting & state[COUNT];
        Wait_next = done_counting & state[COUNT] | ~ack & state[WAIT];
        done = state[WAIT];
        counting = state[COUNT];
        shift_ena = state[B0] | state[B1] | state[B2] | state[B3];

#ifndef NATIVE_SYSTEMC
        o_B3_next.put(B3_next);
        o_S_next.put(S_next);
        o_S1_next.put(S1_next);
        o_Count_next.put(Count_next);
        o_Wait_next.put(Wait_next);
        o_done.put(done);
        o_counting.put(counting);
        o_shift_ena.put(shift_ena);
#else
        o_B3_next.write(B3_next);
        o_S_next.write(S_next);
        o_S1_next.write(S1_next);
        o_Count_next.write(Count_next);
        o_Wait_next.write(Wait_next);
        o_done.write(done);
        o_counting.write(counting);
        o_shift_ena.write(shift_ena);
#endif
    }
}
