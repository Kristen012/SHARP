#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
	i_y.clk_rst(i_clk, i_rst);
    o_Y0.clk_rst(i_clk, i_rst);
    o_z.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		i_y.reset();
		o_Y0.reset();
		o_z.reset();
#endif
		wait();
	}

    enum State {
        S0 = 0,
        S1 = 1,
        S2 = 2,
        S3 = 3,
        S4 = 4
    };

    State state = S0;
    input_t x;
    input_y_t y;
    output_t Y0, z;

    while (true) {
#ifndef NATIVE_SYSTEMC
        x = i_x.get();
        y = i_y.get();
#else
        x = i_x.read();
        y = i_y.read();
#endif

        switch (y) {
            case S0:
                state = x ? S1 : S0;
                break;
            case S1:
                state = x ? S4 : S1;
                break;
            case S2:
                state = x ? S1 : S2;
                break;
            case S3:
                state = x ? S2 : S1;
                break;
            case S4:
                state = x ? S4 : S3;
                break;
        }

        Y0 = (state == S1 || state == S3);
        z = (y == S3 || y == S4);

#ifndef NATIVE_SYSTEMC
        o_Y0.put(Y0);
        o_z.put(z);
#else
        o_Y0.write(Y0);
        o_z.write(z);
#endif
    }
}
