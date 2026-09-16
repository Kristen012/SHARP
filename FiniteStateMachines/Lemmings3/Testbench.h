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
	cynw_p2p<input_t>::base_out i_bump_left;
	cynw_p2p<input_t>::base_out i_bump_right;
	cynw_p2p<input_t>::base_out i_ground;
	cynw_p2p<input_t>::base_out i_dig;
	cynw_p2p<output_t>::base_in o_walk_left;
	cynw_p2p<output_t>::base_in o_walk_right;
	cynw_p2p<output_t>::base_in o_aaah;
	cynw_p2p<output_t>::base_in o_digging;
#else
	sc_fifo_out<input_t> i_bump_left;
	sc_fifo_out<input_t> i_bump_right;
	sc_fifo_out<input_t> i_ground;
	sc_fifo_out<input_t> i_dig;
	sc_fifo_in<output_t> o_walk_left;
	sc_fifo_in<output_t> o_walk_right;
	sc_fifo_in<output_t> o_aaah;
	sc_fifo_in<output_t> o_digging;
#endif

	SC_HAS_PROCESS(Testbench);

	Testbench(sc_module_name n);
	~Testbench() = default;

private:

	void do_feed();
	void do_fetch();

	input_t input_l[INPUT_SIZE];
	input_t input_r[INPUT_SIZE];
	input_t input_g[INPUT_SIZE];
	input_t input_d[INPUT_SIZE];

	sc_time start_time[1];

};

#endif

