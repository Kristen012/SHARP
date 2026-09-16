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
	cynw_p2p<input_t>::base_out i_data;
	cynw_p2p<input_t>::base_out i_done_counting;
	cynw_p2p<input_t>::base_out i_ack;
	cynw_p2p<output_t>::base_in o_shift_ena;
	cynw_p2p<output_t>::base_in o_counting;
	cynw_p2p<output_t>::base_in o_done;
#else
	sc_fifo_out<input_t> i_data;
	sc_fifo_out<input_t> i_done_counting;
	sc_fifo_out<input_t> i_ack;
	sc_fifo_in<output_t> o_shift_ena;
	sc_fifo_in<output_t> o_counting;
	sc_fifo_in<output_t> o_done;
#endif

	SC_HAS_PROCESS(Testbench);

	Testbench(sc_module_name n);
	~Testbench() = default;

private:

	void do_feed();
	void do_fetch();

	input_t input_data[X_SIZE];
	input_t input_done_counting[X_SIZE];
	input_t input_ack[X_SIZE];

	sc_time start_time[1];

};

#endif

