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
	cynw_p2p<input_t>::in i_in;
	cynw_p2p<input_state_t>::in i_state;
	cynw_p2p<output_t>::out o_out1;
	cynw_p2p<output_t>::out o_out2;
	cynw_p2p<output_state_t>::out o_state;
#else
	sc_fifo_in<input_t> i_in;
	sc_fifo_in<input_state_t> i_state;
	sc_fifo_out<output_t> o_out1;
	sc_fifo_out<output_t> o_out2;
	sc_fifo_out<output_state_t> o_state;
#endif

	SC_HAS_PROCESS(Dut);
	Dut(sc_module_name n);
	~Dut() = default;

private:

  	void do_compute();

};
#endif
