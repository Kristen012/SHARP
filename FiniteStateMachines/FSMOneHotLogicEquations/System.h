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
	cynw_p2p<input_t> chan_d;
	cynw_p2p<input_t> chan_done_counting;
	cynw_p2p<input_t> chan_ack;
	cynw_p2p<input_state_t> chan_state;
	cynw_p2p<output_t> chan_B3_next;
	cynw_p2p<output_t> chan_S_next;
	cynw_p2p<output_t> chan_S1_next;
	cynw_p2p<output_t> chan_Count_next;
	cynw_p2p<output_t> chan_Wait_next;
	cynw_p2p<output_t> chan_done;
	cynw_p2p<output_t> chan_counting;
	cynw_p2p<output_t> chan_shift_ena;
#else
	sc_fifo<input_t> chan_d;
	sc_fifo<input_t> chan_done_counting;
	sc_fifo<input_t> chan_ack;
	sc_fifo<input_state_t> chan_state;
	sc_fifo<output_t> chan_B3_next;
	sc_fifo<output_t> chan_S_next;
	sc_fifo<output_t> chan_S1_next;
	sc_fifo<output_t> chan_Count_next;
	sc_fifo<output_t> chan_Wait_next;
	sc_fifo<output_t> chan_done;
	sc_fifo<output_t> chan_counting;
	sc_fifo<output_t> chan_shift_ena;
#endif

};
#endif
