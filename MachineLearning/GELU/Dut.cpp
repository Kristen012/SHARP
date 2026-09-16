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

    while (true) {
#ifndef NATIVE_SYSTEMC
        input_x = i_x.get();
#else
        input_x = i_x.read();
#endif

        fp32_t x = input_x;
        fp32_t in_x = (SQRT_2_OVER_PI * x * (fp32_t(1.0f) + GELU_COEF_A * x * x));
        fp32_t e2x = exp(two * in_x);
        fp32_t t = (e2x - one) / (e2x + one);

        output_t ans = fp32_t(0.5f) * x * (fp32_t(1.0f) + t);

#ifndef NATIVE_SYSTEMC
        o_result.put(ans);
#else
        o_result.write(ans);
#endif
    }
}
