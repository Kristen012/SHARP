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
	tb.i_w(chan_w);
	tb.o_result(chan_result);

	dut.i_s(chan_s);
	dut.i_w(chan_w);
	dut.o_result(chan_result);
}
