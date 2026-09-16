#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_s.clk_rst(i_clk, i_rst);
	i_c.clk_rst(i_clk, i_rst);
	i_d.clk_rst(i_clk, i_rst);
    o_q.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_s.reset();
		i_c.reset();
		i_d.reset();
		o_q.reset();
#endif
		wait();
	}

    input_t shift_ena, count_ena, data;
    output_t q;
    q = 0;

    while (true) {
#ifndef NATIVE_SYSTEMC
        shift_ena = i_s.get();
        count_ena = i_c.get();
        data = i_d.get();
#else
        shift_ena = i_s.read();
        count_ena = i_c.read();
        data = i_d.read();
#endif

        if (shift_ena) {
            q = (q.range(2, 0) << 1) + data;
        }
        else if (count_ena) {
            q = (q == 0) ? 0 : q - 1;
        }
        else {
            q = q;
        }

#ifndef NATIVE_SYSTEMC
        o_q.put(q);
#else
        o_q.write(q);
#endif
    }
}
