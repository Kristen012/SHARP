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
	cynw_p2p<input_t>::in i_s;
	cynw_p2p<input_t>::in i_w;
	cynw_p2p<output_t>::out o_result;
#else
	sc_fifo_in<input_t> i_s;
	sc_fifo_in<input_t> i_w;
	sc_fifo_out<output_t> o_result;
#endif

	SC_HAS_PROCESS(Dut);
	Dut(sc_module_name n);
	~Dut() = default;

private:

  	void do_compute();

};
#endif
