#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_d(chan_d);
	tb.i_done_counting(chan_done_counting);
	tb.i_ack(chan_ack);
	tb.i_state(chan_state);
	tb.o_B3_next(chan_B3_next);
	tb.o_S_next(chan_S_next);
	tb.o_S1_next(chan_S1_next);
	tb.o_Count_next(chan_Count_next);
	tb.o_Wait_next(chan_Wait_next);
	tb.o_done(chan_done);
	tb.o_counting(chan_counting);
	tb.o_shift_ena(chan_shift_ena);

	dut.i_d(chan_d);
	dut.i_done_counting(chan_done_counting);
	dut.i_ack(chan_ack);
	dut.i_state(chan_state);
	dut.o_B3_next(chan_B3_next);
	dut.o_S_next(chan_S_next);
	dut.o_S1_next(chan_S1_next);
	dut.o_Count_next(chan_Count_next);
	dut.o_Wait_next(chan_Wait_next);
	dut.o_done(chan_done);
	dut.o_counting(chan_counting);
	dut.o_shift_ena(chan_shift_ena);
}
