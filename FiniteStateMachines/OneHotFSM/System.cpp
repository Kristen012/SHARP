#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_in(chan_in);
	tb.i_state(chan_state_in);
	tb.o_out1(chan_out1);
	tb.o_out2(chan_out2);
	tb.o_state(chan_state_out);

	dut.i_in(chan_in);
	dut.i_state(chan_state_in);
	dut.o_out1(chan_out1);
	dut.o_out2(chan_out2);
	dut.o_state(chan_state_out);
}
