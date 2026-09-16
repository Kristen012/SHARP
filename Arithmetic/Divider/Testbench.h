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
	cynw_p2p<U16>::base_out i_A;
  	cynw_p2p<U8>::base_out i_B;
	cynw_p2p<U16>::base_in o_Q;
	cynw_p2p<U16>::base_in o_R;
#else
	sc_fifo_out<U16> i_A;
	sc_fifo_out<U8> i_B;
	sc_fifo_in<U16> o_Q;
	sc_fifo_in<U16> o_R;
#endif

  SC_HAS_PROCESS(Testbench);

  Testbench(sc_module_name n);
  ~Testbench() = default;

private:

	void do_feed();
	void do_fetch();

  	U16 A[X_SIZE];
	U8 B[X_SIZE];

	sc_time start_time[1];

};

#endif

