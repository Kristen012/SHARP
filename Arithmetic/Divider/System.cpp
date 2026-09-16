#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_A(chan_A);
	tb.i_B(chan_B);
	tb.o_Q(chan_Q);
	tb.o_R(chan_R);

	dut.i_A(chan_A);
	dut.i_B(chan_B);
	dut.o_Q(chan_Q);
	dut.o_R(chan_R);
}
