#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_A.clk_rst(i_clk, i_rst);
    i_B.clk_rst(i_clk, i_rst);
    o_Q.clk_rst(i_clk, i_rst);
    o_R.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_A.reset();
		i_B.reset();
		o_Q.reset();
		o_R.reset();
#endif
		wait();
	}

    while (true) {
#ifndef NATIVE_SYSTEMC
        U16 a_reg = i_A.get();
        U16 b_reg = i_B.get();
#else
        U16 a_reg = i_A.read();
        U16 b_reg = i_B.read();
#endif

        U32 tmp_a = a_reg;
        U32 tmp_b = b_reg << 16;
        
        if (tmp_b != 0) {
            for (int i = 0; i < 16; i++) {
                tmp_a = tmp_a << 1;
                if (tmp_a >= tmp_b) {
                    tmp_a = tmp_a - tmp_b + 1;
                }
                else {
                    tmp_a = tmp_a;
                }
            }
        }
        else {
            tmp_a = 0;
        }


        U16 q = tmp_a.range(15, 0);
        U16 r = tmp_a.range(31, 16);

#ifndef NATIVE_SYSTEMC
        o_Q.put(q);
        o_R.put(r);
#else
        o_Q.write(q);
        o_R.write(r);
#endif
    }
}
