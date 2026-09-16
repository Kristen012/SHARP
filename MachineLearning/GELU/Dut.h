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

    input_t input_x;

	const fp32_t GELU_COEF_A     = 0.044715f;
	const fp32_t GELU_QUICK_COEF = -1.702f;
	const fp32_t SQRT_2_OVER_PI  = 0.79788456080286535587989211986876f;

	const fp32_t one = fp32_t(1.0f);
	const fp32_t two = fp32_t(2.0f);
	
};
#endif
