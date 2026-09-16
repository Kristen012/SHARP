#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_bump_left(chan_bump_left);
	tb.i_bump_right(chan_bump_right);
	tb.o_walk_left(chan_walk_left);
	tb.o_walk_right(chan_walk_right);

	dut.i_bump_left(chan_bump_left);
	dut.i_bump_right(chan_bump_right);
	dut.o_walk_left(chan_walk_left);
	dut.o_walk_right(chan_walk_right);
}
