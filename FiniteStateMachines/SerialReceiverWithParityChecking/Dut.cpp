#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
    o_byte.clk_rst(i_clk, i_rst);
    o_done.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		o_byte.reset();
		o_done.reset();
#endif
		wait();
	}

    enum State {
        IDLE = 0,
        START = 1,
        S1 = 2,
        S2 = 3,
        S3 = 4,
        S4 = 5,
        S5 = 6,
        S6 = 7,
        S7 = 8,
        S8 = 9,
        CHECK = 10,
        STOP = 11,
        WAIT = 12
    };

    State state = IDLE;
    bool flag, test, par;
    output_t done;
    output_byte_t out_byte, out_reg;

    while (true) {
        input_t in;
#ifndef NATIVE_SYSTEMC
        in = i_x.get();
#else
        in = i_x.read();
#endif


        switch (state) {
            case IDLE:
                state = in ? IDLE : START;
                break;
            case START:
                state = S1;
                out_reg[0] = in;
                break;
            case S1:
                state = S2;
                out_reg[1] = in;
                break;
            case S2:
                state = S3;
                out_reg[2] = in;
                break;
            case S3:
                state = S4;
                out_reg[3] = in;
                break;
            case S4:
                state = S5;
                out_reg[4] = in;
                break;
            case S5:
                state = S6;
                out_reg[5] = in;
                break;
            case S6:
                state = S7;
                out_reg[6] = in;
                break;
            case S7:
                state = S8;
                out_reg[7] = in;
                break;
            case S8:
                state = CHECK;
                flag = in;
                break;
            case CHECK:
                state = in ? STOP : WAIT;
                test = (state == STOP) ? in : test;
                break;
            case STOP:
                state = in ? IDLE : START;
                break;
            case WAIT:
                state = in ? IDLE : WAIT;
                break;
        }

        if (state == STOP) {
            par = 0;
            for (unsigned int i = 0; i < 8; i++) {
                par ^= out_reg[i];
            }
        }
        
        done = (state == STOP && test == (par ^ flag));
        out_byte = done ? out_reg : sc_uint<8>(0);

#ifndef NATIVE_SYSTEMC
        o_byte.put(out_byte);
        o_done.put(done);
#else
        o_byte.write(out_byte);
        o_done.write(done);
#endif
    }
}
