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
	tb.o_outp(chan_outp);
	tb.o_overflw(chan_overflw);

	dut.i_x1(chan_x1);
	dut.i_x2(chan_x2);
	dut.o_outp(chan_outp);
	dut.o_overflw(chan_overflw);
}
