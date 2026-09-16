#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#include "Defines.h"

#ifndef NATIVE_SYSTEMC
#include <esc.h>
#include <cynw_p2p.h>
#include <stratus_hls.h>
#include "Dut_wrap.h"
#else
#include "Dut.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n );
	~System() = default;
private:
  	Testbench tb;
#ifndef NATIVE_SYSTEMC
	Dut_wrapper dut;
#else
	Dut dut;
#endif

	sc_clock clk;
	sc_signal<bool> rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p<input_t> chan_s;
	cynw_p2p<input_t> chan_c;
	cynw_p2p<input_t> chan_d;
	cynw_p2p<output_t> chan_q;
#else
	sc_fifo<input_t> chan_s;
	sc_fifo<input_t> chan_c;
	sc_fifo<input_t> chan_d;
	sc_fifo<output_t> chan_q;
#endif

};
#endif
