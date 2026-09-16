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

fp32_t Dut::taylor_exp_minus(fp32_t x) {
    fp32_t x2 = x * x;
    fp32_t x3 = x2 * x;

    return one - x + (x2 >> 1) - (x3 * fp32_t(0.166666));
}

fp32_t Dut::exp_fixed(fp32_t x) {
    if (x == fp32_t(0.0)) return fp32_t(1.0);

    if (x < fp32_t(0.0)) x = -x;

    if (x > fp32_t(8.0)) return fp32_t(0.0);

    int xi = x.to_int();
    fp32_t x_remain = x - fp32_t(xi);

    int frac_idx = (x_remain * fp32_t(16.0)).to_int();
    fp32_t frac_hi = fp32_t(frac_idx) / fp32_t(16.0);
    fp32_t frac_lo = x_remain - frac_hi;

    fp32_t e_int  = exp_int_lut[xi];
    fp32_t e_frac = exp_frac_lut[frac_idx];
    fp32_t e_tail = taylor_exp_minus(frac_lo);

    return e_int * e_frac * e_tail;
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

    input_t max_val;

    while (true) {
        for(int i = 0; i < X_SIZE; i++){
#ifndef NATIVE_SYSTEMC
            input_t in_val = i_x.get();
#else
            input_t in_val = i_x.read();
#endif
            input_x[i] = in_val;
			if (i == 0) max_val = in_val;
            if (in_val > max_val) max_val = in_val;
        }

        fp32_t  sum_exp = 0;
        for (int i = 0; i < X_SIZE; i++) {
            fp32_t shifted = input_x[i] - max_val;
            fp32_t e = exp_fixed(shifted);
            exp_buf[i] = e;
            sum_exp += e;
        }

        input_t inv_sum = one / sum_exp;

        for (int i = 0; i < RESULT_SIZE; i++) {
            output_t out_val = exp_buf[i] * inv_sum;
#ifndef NATIVE_SYSTEMC
            o_result.put(out_val);
#else
            o_result.write(out_val);
#endif
        }
    }
}
