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
	cynw_p2p<input_t>::base_out i_x;
  	cynw_p2p<input_t>::base_out i_w;
  	cynw_p2p<input_t>::base_out i_idx;
	cynw_p2p<output_t>::base_in o_result;
#else
	sc_fifo_out<input_t> i_x;
	sc_fifo_out<input_t> i_w;
	sc_fifo_out<input_t> i_idx;
	sc_fifo_in<output_t> o_result;
#endif

	SC_HAS_PROCESS(Testbench);

	Testbench(sc_module_name n);
	~Testbench() = default;

private:

	void do_feed();
	void do_fetch();

	sc_time start_time[1];

	input_t input_w[MAT_W_ROW_SIZE][MAT_W_COL_SIZE];
	input_t input_x[MAT_X_ROW_SIZE][MAT_X_COL_SIZE];

};

#endif

