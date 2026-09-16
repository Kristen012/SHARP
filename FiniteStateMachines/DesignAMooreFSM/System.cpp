#include "System.h"
System::System( sc_module_name n ): sc_module( n ), 
	tb("tb"), dut("dut"),
	clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	dut.i_clk(clk);
	dut.i_rst(rst);

	tb.i_s(chan_s);
	tb.o_fr3(chan_fr3);
	tb.o_fr2(chan_fr2);
	tb.o_fr1(chan_fr1);
	tb.o_dfr(chan_dfr);

	dut.i_s(chan_s);
	dut.o_fr3(chan_fr3);
	dut.o_fr2(chan_fr2);
	dut.o_fr1(chan_fr1);
	dut.o_dfr(chan_dfr);
}
