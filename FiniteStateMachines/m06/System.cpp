#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_x1(chan_x1);
	tb.i_x2(chan_x2);
	tb.i_x3(chan_x3);
	tb.i_x4(chan_x4);
	tb.o_outp(chan_outp);

	dut.i_x1(chan_x1);
	dut.i_x2(chan_x2);
	dut.i_x3(chan_x3);
	dut.i_x4(chan_x4);
	dut.o_outp(chan_outp);
}
