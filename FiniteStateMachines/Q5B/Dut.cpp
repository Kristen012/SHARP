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
        A = 0,
        B = 1
    };

    State state = A;
    input_t x;
    output_t z;

    while (true) {
#ifndef NATIVE_SYSTEMC
        x = i_x.get();
#else
        x = i_x.read();
#endif

        z = (state == A) & x | (state == B) & ~x;

        switch (state) {
            case A:
                state = x ? B : A;
                break;
            case B:
                state = B;
                break;
        }

#ifndef NATIVE_SYSTEMC
        o_result.put(z);
#else
        o_result.write(z);
#endif
    }
}
