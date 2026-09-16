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
	tb.i_rotation(chan_rotation);
	tb.o_result(chan_result);

	dut.i_x(chan_x);
	dut.i_rotation(chan_rotation);
	dut.o_result(chan_result);
}
