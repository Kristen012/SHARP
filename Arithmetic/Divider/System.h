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
	cynw_p2p<U16> chan_A;
	cynw_p2p<U8> chan_B;
	cynw_p2p<U16> chan_Q;
	cynw_p2p<U16> chan_R;
#else
	sc_fifo<U16> chan_A;
    sc_fifo<U8> chan_B;
    sc_fifo<U16> chan_Q;
    sc_fifo<U16> chan_R;
#endif

};
#endif
