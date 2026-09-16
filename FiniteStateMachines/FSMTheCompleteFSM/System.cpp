#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_data(chan_data);
	tb.i_done_counting(chan_done_counting);
	tb.i_ack(chan_ack);
	tb.o_shift_ena(chan_shift_ena);
	tb.o_counting(chan_counting);
	tb.o_done(chan_done);

	dut.i_data(chan_data);
	dut.i_done_counting(chan_done_counting);
	dut.i_ack(chan_ack);
	dut.o_shift_ena(chan_shift_ena);
	dut.o_counting(chan_counting);
	dut.o_done(chan_done);
}
