#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_in.clk_rst(i_clk, i_rst);
	i_state.clk_rst(i_clk, i_rst);
    o_out1.clk_rst(i_clk, i_rst);
    o_out2.clk_rst(i_clk, i_rst);
    o_state.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_in.reset();
		i_state.reset();
		o_out1.reset();
		o_out2.reset();
		o_state.reset();
#endif
		wait();
	}

    enum State {
        S0 = 0,
        S1 = 1,
        S2 = 2,
        S3 = 3,
        S4 = 4,
        S5 = 5,
        S6 = 6,
        S7 = 7,
        S8 = 8,
        S9 = 9
    };

    while (true) {
        input_t in;
        input_state_t state_in;
#ifndef NATIVE_SYSTEMC
        in = i_in.get();
        state_in = i_state.get();
#else
        in = i_in.read();
        state_in = i_state.read();
#endif

        output_t out1, out2;
        output_state_t state_out;

        state_out[S0] = ~in & (state_in[S0] | state_in[S1] | state_in[S2] | state_in[S3] | state_in[S4] | state_in[S7] | state_in[S8] | state_in[S9]);
        state_out[S1] = in & (state_in[S0] | state_in[S8] | state_in[S9]);
        state_out[S2] = in & state_in[S1];
        state_out[S3] = in & state_in[S2];
        state_out[S4] = in & state_in[S3];
        state_out[S5] = in & state_in[S4];
        state_out[S6] = in & state_in[S5];
        state_out[S7] = in & (state_in[S6] | state_in[S7]);
        state_out[S8] = ~in & state_in[S5];
        state_out[S9] = ~in & state_in[S6];

        out1 = state_in[S8] | state_in[S9];
        out2 = state_in[S7] | state_in[S9];

#ifndef NATIVE_SYSTEMC
        o_out1.put(out1);
        o_out2.put(out2);
        o_state.put(state_out);
#else
        o_out1.write(out1);
        o_out2.write(out2);
        o_state.write(state_out);
#endif
    }
}
