#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_s.clk_rst(i_clk, i_rst);
    o_fr3.clk_rst(i_clk, i_rst);
    o_fr2.clk_rst(i_clk, i_rst);
    o_fr1.clk_rst(i_clk, i_rst);
    o_dfr.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_s.reset();
		o_fr3.reset();
		o_fr2.reset();
		o_fr1.reset();
		o_dfr.reset();
#endif
		wait();
	}

    enum State {
        A2 = 0,
        B1 = 1,
        B2 = 2,
        C1 = 3,
        C2 = 4,
        D1 = 5
    };

    State state = A2;

    while (true) {
        input_t s;
#ifndef NATIVE_SYSTEMC
        s = i_s.get();
#else
        s = i_s.read();
#endif

        switch (state) {
            case A2:
                state = s[0] ? B1 : A2;
                break;
            case B1:
                state = s[1] ? C1 : (s[0] ? B1 : A2);
                break;
            case B2:
                state = s[1] ? C1 : (s[0] ? B2 : A2);
                break;
            case C1:
                state = s[2] ? D1 : (s[1] ? C1 : B2);
                break;
            case C2:
                state = s[2] ? D1 : (s[1] ? C2 : B2);
                break;
            case D1:
                state = s[2] ? D1 : C2;
                break;
        }

        output_t out_fr3, out_fr2, out_fr1, out_dfr;
        switch (state) {
            case A2:
                out_fr3 = 1;
                out_fr2 = 1;
                out_fr1 = 1;
                out_dfr = 1;
                break;
            case B1:
                out_fr3 = 0;
                out_fr2 = 1;
                out_fr1 = 1;
                out_dfr = 0;
                break;
            case B2:
                out_fr3 = 0;
                out_fr2 = 1;
                out_fr1 = 1;
                out_dfr = 1;
                break;
            case C1:
                out_fr3 = 0;
                out_fr2 = 0;
                out_fr1 = 1;
                out_dfr = 0;
                break;
            case C2:
                out_fr3 = 0;
                out_fr2 = 0;
                out_fr1 = 1;
                out_dfr = 1;
                break;
            case D1:
                out_fr3 = 0;
                out_fr2 = 0;
                out_fr1 = 0;
                out_dfr = 0;
                break;
        }

#ifndef NATIVE_SYSTEMC
        o_fr3.put(out_fr3);
        o_fr2.put(out_fr2);
        o_fr1.put(out_fr1);
        o_dfr.put(out_dfr);
#else
        o_fr3.write(out_fr3);
        o_fr2.write(out_fr2);
        o_fr1.write(out_fr1);
        o_dfr.write(out_dfr);
#endif
    }
}
