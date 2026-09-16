#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_s(chan_s);
	tb.i_c(chan_c);
	tb.i_d(chan_d);
	tb.o_q(chan_q);

	dut.i_s(chan_s);
	dut.i_c(chan_c);
	dut.i_d(chan_d);
	dut.o_q(chan_q);
}
