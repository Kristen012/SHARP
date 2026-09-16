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
	cynw_p2p<U16>::in i_A;
  	cynw_p2p<U8>::in i_B;
	cynw_p2p<U16>::out o_Q;
	cynw_p2p<U16>::out o_R;
#else
	sc_fifo_in<U16> i_A;
	sc_fifo_in<U8> i_B;
	sc_fifo_out<U16> o_Q;
	sc_fifo_out<U16> o_R;
#endif

	SC_HAS_PROCESS(Dut);
	Dut(sc_module_name n);
	~Dut() = default;

private:

  	void do_compute();

};
#endif
