#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
    i_w.clk_rst(i_clk, i_rst);
    i_idx.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		i_w.reset();
		i_idx.reset();
		o_result.reset();
#endif
		wait();
	}

    for(int j = 0; j < MAT_X_COL_SIZE ; j++) {
        for(int i = 0; i < MAT_W_ROW_SIZE ; i++) {
            result[i][j] = 0;
        }
    }

    int index;
    while (true) {

#ifndef NATIVE_SYSTEMC
        index = i_idx.get();
#else
        index = i_idx.read();
#endif

        for (unsigned int i = 0; i < MAT_W_ROW_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            input_w[i] = i_w.get();
#else
            input_w[i] = i_w.read();
#endif
        }
        for (unsigned int i = 0; i < MAT_X_COL_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            input_x[i] = i_x.get();
#else
            input_x[i] = i_x.read();
#endif
        }

        for(int j = 0; j < MAT_X_COL_SIZE ; j++) {
            for(int i = 0; i < MAT_W_ROW_SIZE ; i++) {
                result[i][j] += input_w[i] * input_x[j];
            }
        }

        
        if (index == MAT_X_ROW_SIZE - 1)
        {
            for (int i = 0; i < MAT_R_ROW_SIZE; i++) {
                for (int j = 0; j < MAT_R_COL_SIZE; j++) {
#ifndef NATIVE_SYSTEMC
                    o_result.put(result[i][j]);
#else
                    o_result.write(result[i][j]);
#endif
                }
            }
        }
    }
}
