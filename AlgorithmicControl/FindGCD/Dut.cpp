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

    while (true) {
        input_t input_x1, input_x2;
        #ifndef NATIVE_SYSTEMC
                input_x1 = i_x1.get();
                input_x2 = i_x2.get();
        #else
                input_x1 = i_x1.read();
                input_x2 = i_x2.read();
        #endif

        output_t result;
        result = (input_x1 < input_x2) ? input_x1 : input_x2;
        while (result > 1) {
            if ((input_x1 % result == 0) && (input_x2 % result == 0)) {
                break;
            }
            result--;
        }
        
#ifndef NATIVE_SYSTEMC
        o_result.put(result);
#else
        o_result.write(result);
#endif
    }
}
