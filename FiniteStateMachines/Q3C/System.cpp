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
	tb.o_Y0(chan_Y0);
	tb.o_z(chan_z);

	dut.i_x(chan_x);
	dut.i_y(chan_y);
	dut.o_Y0(chan_Y0);
	dut.o_z(chan_z);
}
