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

    sum = 0;
    for (unsigned int i = 0; i < SIZE; i++) {
        input_x[i] = 0;
    }

    while (true) {
        for (unsigned int i = SIZE - 1; i > 0; i--) {
            input_x[i] = input_x[i-1];
        }

#ifndef NATIVE_SYSTEMC
        input_x[0] = i_x.get();
#else
        input_x[0] = i_x.read();
#endif
        
        sum = input_x[0];
        for (unsigned int i = 1; i < SIZE; i++) {
            sum += input_x[i];
        }

#ifndef NATIVE_SYSTEMC
        o_result.put(sum/SIZE);
#else
        o_result.write(sum/SIZE);
#endif
    }
}
