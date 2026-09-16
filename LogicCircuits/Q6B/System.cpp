#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_y(chan_y);
	tb.i_w(chan_w);
	tb.o_Y2(chan_Y2);

	dut.i_y(chan_y);
	dut.i_w(chan_w);
	dut.o_Y2(chan_Y2);
}
