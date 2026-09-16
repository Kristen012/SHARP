#ifndef DUT_H_
#define DUT_H_

#include <systemc>
using namespace sc_core;
#include "Defines.h"

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Dut : public sc_module {
public:
	sc_in_clk i_clk;
	sc_in<bool> i_rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p<input_t>::in i_x;
	cynw_p2p<output_t>::out o_result;
#else
	sc_fifo_in<input_t> i_x;
	sc_fifo_out<output_t> o_result;
#endif

	SC_HAS_PROCESS(Dut);
	Dut(sc_module_name n);
	~Dut() = default;

private:

  	void do_compute();
	fp32_t exp_fixed(fp32_t x);
	fp32_t taylor_exp_minus(fp32_t x);

	const fp32_t exp_int_lut[9] = {
		fp32_t(1.000000),
		fp32_t(0.367879),
		fp32_t(0.135335),
		fp32_t(0.049787),
		fp32_t(0.018316),
		fp32_t(0.006738),
		fp32_t(0.002479),
		fp32_t(0.000912),
		fp32_t(0.000335)
	};

	const fp32_t exp_frac_lut[16] = {
		fp32_t(1.000000),
		fp32_t(0.939413),
		fp32_t(0.882497),
		fp32_t(0.829029),
		fp32_t(0.778801),
		fp32_t(0.731616),
		fp32_t(0.687289),
		fp32_t(0.645648),
		fp32_t(0.606531),
		fp32_t(0.569783),
		fp32_t(0.535261),
		fp32_t(0.502786),
		fp32_t(0.472367),
		fp32_t(0.443748),
		fp32_t(0.416862),
		fp32_t(0.391606)
	};

	const fp32_t GELU_COEF_A     = 0.044715f;
	const fp32_t GELU_QUICK_COEF = -1.702f;
	const fp32_t SQRT_2_OVER_PI  = 0.79788456080286535587989211986876f;
	const fp32_t one = fp32_t(1.0f);

	input_t input_x[X_SIZE];
	fp32_t exp_buf[X_SIZE];

};
#endif
