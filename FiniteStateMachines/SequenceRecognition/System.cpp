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
	tb.o_disc(chan_disc);
	tb.o_flag(chan_flag);
	tb.o_err(chan_err);

	dut.i_x(chan_x);
	dut.o_disc(chan_disc);
	dut.o_flag(chan_flag);
	dut.o_err(chan_err);
}
