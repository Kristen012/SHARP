#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
    i_rotation.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {

    {
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
        i_x.reset();
        i_rotation.reset();
        o_result.reset();
#endif
        wait();
    }

    while (true) {

        input_t arr[ARRAY_SIZE];
#ifndef NATIVE_SYSTEMC
        for (int i = 0; i < ARRAY_SIZE; i++) {
            arr[i] = i_x.get();
        }

        input_t rotation = i_rotation.get();
#else
        for (int i = 0; i < ARRAY_SIZE; i++) {
            arr[i] = i_x.read();
        }
        input_t rotation = i_rotation.read();
#endif

        input_t temp[ARRAY_SIZE];
        int k = 0;

        for (int i = rotation; i < ARRAY_SIZE; i++) {
            temp[k++] = arr[i];
        }
        for (int i = 0; i < rotation; i++) {
            temp[k++] = arr[i];
        }
        for (int i = 0; i < ARRAY_SIZE; i++) {        
#ifndef NATIVE_SYSTEMC
		    o_result.put(temp[i]);
#else
		    o_result.write(temp[i]);
#endif
        }

    }

}
