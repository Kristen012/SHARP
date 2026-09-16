#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
    o_disc.clk_rst(i_clk, i_rst);
    o_flag.clk_rst(i_clk, i_rst);
    o_err.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		o_disc.reset();
		o_flag.reset();
		o_err.reset();
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
        DISC = 7,
        FLAG = 8,
        ERR = 9
    };

    State state = S0;

    input_t in;
    output_t disc, flag, err;

    while (true) {
#ifndef NATIVE_SYSTEMC
        in = i_x.get();
#else
        in = i_x.read();
#endif

        switch (state) {
            case S0:
                state = in ? S1 : S0;
                break;
            case S1:
                state = in ? S2 : S0;
                break;
            case S2:
                state = in ? S3 : S0;
                break;
            case S3:
                state = in ? S4 : S0;
                break;
            case S4:
                state = in ? S5 : S0;
                break;
            case S5:
                state = in ? S6 : DISC;
                break;
            case S6:
                state = in ? ERR : FLAG;
                break;
            case DISC:
                state = in ? S1 : S0;
                break;
            case FLAG:
                state = in ? S1 : S0;
                break;
            case ERR:
                state = in ? ERR : S0;
                break;
        }
        
        disc = (state == DISC);
        flag = (state == FLAG);
        err = (state == ERR);

#ifndef NATIVE_SYSTEMC
        o_disc.put(disc);
        o_flag.put(flag);
        o_err.put(err);
#else
        o_disc.write(disc);
        o_flag.write(flag);
        o_err.write(err);
#endif
    }
}
