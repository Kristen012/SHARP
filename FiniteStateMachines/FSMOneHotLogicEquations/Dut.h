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
	cynw_p2p<input_t>::in i_d;
	cynw_p2p<input_t>::in i_done_counting;
	cynw_p2p<input_t>::in i_ack;
	cynw_p2p<input_state_t>::in i_state;
	cynw_p2p<output_t>::out o_B3_next;
	cynw_p2p<output_t>::out o_S_next;
	cynw_p2p<output_t>::out o_S1_next;
	cynw_p2p<output_t>::out o_Count_next;
	cynw_p2p<output_t>::out o_Wait_next;
	cynw_p2p<output_t>::out o_done;
	cynw_p2p<output_t>::out o_counting;
	cynw_p2p<output_t>::out o_shift_ena;
#else
	sc_fifo_in<input_t> i_d;
	sc_fifo_in<input_t> i_done_counting;
	sc_fifo_in<input_t> i_ack;
	sc_fifo_in<input_state_t> i_state;
	sc_fifo_out<output_t> o_B3_next;
	sc_fifo_out<output_t> o_S_next;
	sc_fifo_out<output_t> o_S1_next;
	sc_fifo_out<output_t> o_Count_next;
	sc_fifo_out<output_t> o_Wait_next;
	sc_fifo_out<output_t> o_done;
	sc_fifo_out<output_t> o_counting;
	sc_fifo_out<output_t> o_shift_ena;
#endif

	SC_HAS_PROCESS(Dut);
	Dut(sc_module_name n);
	~Dut() = default;

private:

  	void do_compute();

};
#endif
