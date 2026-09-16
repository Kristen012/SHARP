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
	tb.o_Y1(chan_Y1);
	tb.o_Y3(chan_Y3);

	dut.i_y(chan_y);
	dut.i_w(chan_w);
	dut.o_Y1(chan_Y1);
	dut.o_Y3(chan_Y3);
}
