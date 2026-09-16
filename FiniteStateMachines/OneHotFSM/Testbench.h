#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <systemc>
using namespace sc_core;
#include "Defines.h"

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Testbench : public sc_module {
public:
	sc_in_clk i_clk;
	sc_out<bool> o_rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p<input_t>::base_out i_in;
	cynw_p2p<input_state_t>::base_out i_state;
	cynw_p2p<output_t>::base_in o_out1;
	cynw_p2p<output_t>::base_in o_out2;
	cynw_p2p<output_state_t>::base_in o_state;
#else
	sc_fifo_out<input_t> i_in;
	sc_fifo_out<input_state_t> i_state;
	sc_fifo_in<output_t> o_out1;
	sc_fifo_in<output_t> o_out2;
	sc_fifo_in<output_state_t> o_state;
#endif

	SC_HAS_PROCESS(Testbench);

	Testbench(sc_module_name n);
	~Testbench() = default;

private:

	void do_feed();
	void do_fetch();

	input_t input_in[X_SIZE];
	input_state_t input_state[X_SIZE];

	sc_time start_time[1];

};

#endif

