#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
    o_bytes.clk_rst(i_clk, i_rst);
    o_done.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		o_bytes.reset();
		o_done.reset();
#endif
		wait();
	}

    enum State {
        S1 = 0,
        S2 = 1,
        S3 = 2,
        DONE = 3
    };

    State state = S1;
    output_t done;
    output_bytes_t out_bytes, out_bytes_reg;

    while (true) {
        input_t in;
#ifndef NATIVE_SYSTEMC
        in = i_x.get();
#else
        in = i_x.read();
#endif

        done = (state == DONE);
        out_bytes = done ? out_bytes_reg : sc_uint<24>(0);
        if (state == S1 || state == DONE) {
            out_bytes_reg.range(23, 16) = in;
        }
        else if (state == S2) {
            out_bytes_reg.range(15, 8) = in;
        }
        else {
            out_bytes_reg.range(7, 0) = in;
        }

        switch (state) {
            case S1:
                state = in[3] ? S2 : S1;
                break;
            case S2:
                state = S3;
                break;
            case S3:
                state = DONE;
                break;
            case DONE:
                state = in[3] ? S2 : S1;
                break;
        }

#ifndef NATIVE_SYSTEMC
        o_bytes.put(out_bytes);
        o_done.put(done);
#else
        o_bytes.write(out_bytes);
        o_done.write(done);
#endif
    }
}
