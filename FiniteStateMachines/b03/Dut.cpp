#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x1.clk_rst(i_clk, i_rst);
    i_x2.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x1.reset();
		i_x2.reset();
		o_result.reset();
#endif
		wait();
	}

    enum State {
        S0, S1, S2, S3, S4, S5, S6, S7,
        S8, S9, S10, S11, S12, S13, S14, S15,
        S16, S17, S18, S19, S20, S21, S22, S23,
        S24, S25, S26, S27, S28, S29, S30, S31
    };
    State state = S0;

    while (true) {
        input_t input1, input2;

        #ifndef NATIVE_SYSTEMC
                input1 = i_x1.get();
                input2 = i_x2.get();
        #else
                input1 = i_x1.read();
                input2 = i_x2.read();
        #endif

        switch (state) {
            case S0:
                state = (input1 && input2) ? S1 : S2;
                break;
            case S1:
                state = (!input1 && input2) ? S3 : S4;
                break;
            case S2:
                state = (input1 && !input2) ? S5 : S6;
                break;
            case S3:
                state = (!input1 && !input2) ? S7 : S8;
                break;
            case S4:
                state = (input1 || input2) ? S9 : S10;
                break;
            case S5:
                state = (!input1 || input2) ? S11 : S12;
                break;
            case S6:
                state = (input1 || !input2) ? S13 : S14;
                break;
            case S7:
                state = (!input1 || !input2) ? S15 : S16;
                break;
            case S8:
                state = (input1 && input2) ? S17 : S18;
                break;
            case S9:
                state = (!input1 && input2) ? S19 : S20;
                break;
            case S10:
                state = (input1 && !input2) ? S21 : S22;
                break;
            case S11:
                state = (!input1 && !input2) ? S23 : S24;
                break;
            case S12:
                state = (input1 || input2) ? S25 : S26;
                break;
            case S13:
                state = (!input1 || input2) ? S27 : S28;
                break;
            case S14:
                state = (input1 || !input2) ? S29 : S30;
                break;
            case S15:
                state = (!input1 || !input2) ? S31 : S0;
                break;
            case S16:
                state = (input1 && input2) ? S1 : S2;
                break;
            case S17:
                state = (!input1 && input2) ? S3 : S4;
                break;
            case S18:
                state = (input1 && !input2) ? S5 : S6;
                break;
            case S19:
                state = (!input1 && !input2) ? S7 : S8;
                break;
            case S20:
                state = (input1 || input2) ? S9 : S10;
                break;
            case S21:
                state = (!input1 || input2) ? S11 : S12;
                break;
            case S22:
                state = (input1 || !input2) ? S13 : S14;
                break;
            case S23:
                state = (!input1 || !input2) ? S15 : S16;
                break;
            case S24:
                state = (input1 && input2) ? S17 : S18;
                break;
            case S25:
                state = (!input1 && input2) ? S19 : S20;
                break;
            case S26:
                state = (input1 && !input2) ? S21 : S22;
                break;
            case S27:
                state = (!input1 && !input2) ? S23 : S24;
                break;
            case S28:
                state = (input1 || input2) ? S25 : S26;
                break;
            case S29:
                state = (!input1 || input2) ? S27 : S28;
                break;
            case S30:
                state = (input1 || !input2) ? S29 : S30;
                break;
            case S31:
                state = (!input1 || !input2) ? S31 : S0;
                break;
        }

        output_t out_state;
        out_state = state;
        
#ifndef NATIVE_SYSTEMC
        o_result.put(out_state);
#else
        o_result.write(out_state);
#endif
    }
}
