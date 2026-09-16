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
	cynw_p2p<input_t>::in i_y;
	cynw_p2p<input_t>::in i_resetn;
	cynw_p2p<output_t>::out o_f;
	cynw_p2p<output_t>::out o_g;
#else
	sc_fifo_in<input_t> i_x;
	sc_fifo_in<input_t> i_y;
	sc_fifo_in<input_t> i_resetn;
	sc_fifo_out<output_t> o_f;
	sc_fifo_out<output_t> o_g;
#endif

	SC_HAS_PROCESS(Dut);
	Dut(sc_module_name n);
	~Dut() = default;

private:

  	void do_compute();

};
#endif
