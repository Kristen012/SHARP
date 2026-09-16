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
	cynw_p2p<input_t> chan_in;
	cynw_p2p<input_state_t> chan_state_in;
	cynw_p2p<output_t> chan_out1;
	cynw_p2p<output_t> chan_out2;
	cynw_p2p<output_state_t> chan_state_out;
#else
	sc_fifo<input_t> chan_in;
	sc_fifo<input_state_t> chan_state_in;
	sc_fifo<output_t> chan_out1;
	sc_fifo<output_t> chan_out2;
	sc_fifo<output_state_t> chan_state_out;
#endif

};
#endif
