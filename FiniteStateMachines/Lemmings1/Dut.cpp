#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_bump_left.clk_rst(i_clk, i_rst);
	i_bump_right.clk_rst(i_clk, i_rst);
    o_walk_left.clk_rst(i_clk, i_rst);
    o_walk_right.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_bump_left.reset();
		i_bump_right.reset();
		o_walk_left.reset();
		o_walk_right.reset();
#endif
		wait();
	}

    enum State {
        LEFT = 0,
        RIGHT = 1
    };

    State state = LEFT;

    while (true) {
        input_t bump_left, bump_right;
#ifndef NATIVE_SYSTEMC
        bump_left = i_bump_left.get();
        bump_right = i_bump_right.get();
#else
        bump_left = i_bump_left.read();
        bump_right = i_bump_right.read();
#endif

        switch (state) {
            case LEFT:
                state = bump_left ? RIGHT : LEFT;
                break;
            case RIGHT:
                state = bump_right? LEFT : RIGHT;
                break;
        }

        output_t walk_left, walk_right;
        walk_left = (state == LEFT);
        walk_right = (state == RIGHT);

#ifndef NATIVE_SYSTEMC
        o_walk_left.put(walk_left);
        o_walk_right.put(walk_right);
#else
        o_walk_left.write(walk_left);
        o_walk_right.write(walk_right);
#endif
    }
}
