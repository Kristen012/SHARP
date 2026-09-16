#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_x(chan_x);
	tb.i_y(chan_y);
	tb.i_resetn(chan_resetn);
	tb.o_f(chan_f);
	tb.o_g(chan_g);

	dut.i_x(chan_x);
	dut.i_y(chan_y);
	dut.i_resetn(chan_resetn);
	dut.o_f(chan_f);
	dut.o_g(chan_g);
}
