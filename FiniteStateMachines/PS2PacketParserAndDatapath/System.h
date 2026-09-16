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
	cynw_p2p<input_t> chan_x;
	cynw_p2p<output_bytes_t> chan_bytes;
	cynw_p2p<output_t> chan_done;
#else
	sc_fifo<input_t> chan_x;
    sc_fifo<output_bytes_t> chan_bytes;
    sc_fifo<output_t> chan_done;
#endif

};
#endif
