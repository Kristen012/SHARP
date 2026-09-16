#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_s.clk_rst(i_clk, i_rst);
	i_w.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_s.reset();
		i_w.reset();
		o_result.reset();
#endif
		wait();
	}

    enum State {
        A = 0,
        B = 1
    };

    State state = A;
    input_t s, w;
    bool w_reg1, w_reg2;
    int counter = 0;
    output_t z;

    while (true) {
#ifndef NATIVE_SYSTEMC
        s = i_s.get();
        w = i_w.get();
#else
        s = i_s.read();
        w = i_w.read();
#endif

        if (state == B && counter == 0) {
            if (~w & w_reg1 & w_reg2 | w & ~w_reg1 & w_reg2 | w & w_reg1 & ~w_reg2) {
                z = 1;
            }
            else {
                z = 0;
            }
        }
        else {
            z = 0;
        }

        switch (state) {
            case A:
                state = s ? B : A;
                w_reg2 = s ? w_reg1 : 0;
                w_reg1 = s ? w : 0;
                break;
            case B:
                state = B;
                w_reg2 = w_reg1;
                w_reg1 = w;
                break;
        }

        if (counter == 2) {
            counter = 0;
        }
        else if (state == B) {
            counter += 1;
        }
        else {
            counter = 0;
        }

#ifndef NATIVE_SYSTEMC
        o_result.put(z);
#else
        o_result.write(z);
#endif
    }
}
