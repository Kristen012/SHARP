#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_bump_left.clk_rst(i_clk, i_rst);
	i_bump_right.clk_rst(i_clk, i_rst);
	i_ground.clk_rst(i_clk, i_rst);
	i_dig.clk_rst(i_clk, i_rst);
    o_walk_left.clk_rst(i_clk, i_rst);
    o_walk_right.clk_rst(i_clk, i_rst);
    o_aaah.clk_rst(i_clk, i_rst);
    o_digging.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_bump_left.reset();
		i_bump_right.reset();
		i_ground.reset();
		i_dig.reset();
		o_walk_left.reset();
		o_walk_right.reset();
		o_aaah.reset();
		o_digging.reset();
#endif
		wait();
	}

    enum State {
        LEFT = 0,
        RIGHT = 1,
        GROUND_LEFT = 2,
        GROUND_RIGHT = 3,
        DIGGING_LEFT = 4,
        DIGGING_RIGHT = 5
    };

    State state = LEFT;

    while (true) {
        input_t bump_left, bump_right, ground, dig;
#ifndef NATIVE_SYSTEMC
        bump_left = i_bump_left.get();
        bump_right = i_bump_right.get();
        ground = i_ground.get();
        dig = i_dig.get();
#else
        bump_left = i_bump_left.read();
        bump_right = i_bump_right.read();
        ground = i_ground.read();
        dig = i_dig.read();
#endif

        switch (state) {
            case LEFT:
                state = ground ? (dig ? DIGGING_LEFT : (bump_left ? RIGHT : LEFT)) : GROUND_LEFT;
                break;
            case RIGHT:
                state = ground ? (dig ? DIGGING_RIGHT : (bump_right ? LEFT : RIGHT)) : GROUND_RIGHT;
                break;
            case GROUND_LEFT:
                state = ground ? LEFT : GROUND_LEFT;
                break;
            case GROUND_RIGHT:
                state = ground ? RIGHT : GROUND_RIGHT;
                break;
            case DIGGING_LEFT:
                state = ground ? DIGGING_LEFT : GROUND_LEFT;
                break;
            case DIGGING_RIGHT:
                state = ground ? DIGGING_RIGHT : GROUND_RIGHT;
                break;
        }

        output_t walk_left, walk_right, aaah, digging;
        walk_left = (state == LEFT);
        walk_right = (state == RIGHT);
        aaah = (state == GROUND_LEFT || state == GROUND_RIGHT);
        digging = (state == DIGGING_LEFT || state == DIGGING_RIGHT);

#ifndef NATIVE_SYSTEMC
        o_walk_left.put(walk_left);
        o_walk_right.put(walk_right);
        o_aaah.put(aaah);
        o_digging.put(digging);
#else
        o_walk_left.write(walk_left);
        o_walk_right.write(walk_right);
        o_aaah.write(aaah);
        o_digging.write(digging);
#endif
    }
}
