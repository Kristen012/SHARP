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
	tb.i_ground(chan_ground);
	tb.i_dig(chan_dig);
	tb.o_walk_left(chan_walk_left);
	tb.o_walk_right(chan_walk_right);
	tb.o_aaah(chan_aaah);
	tb.o_digging(chan_digging);

	dut.i_bump_left(chan_bump_left);
	dut.i_bump_right(chan_bump_right);
	dut.i_ground(chan_ground);
	dut.i_dig(chan_dig);
	dut.o_walk_left(chan_walk_left);
	dut.o_walk_right(chan_walk_right);
	dut.o_aaah(chan_aaah);
	dut.o_digging(chan_digging);
}
