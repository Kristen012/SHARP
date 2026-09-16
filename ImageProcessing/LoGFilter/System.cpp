#include "System.h"
System::System( sc_module_name n, string input_bmp, string output_bmp ): sc_module( n ), 
	tb("tb"), gaussian_filter("gaussian_filter"), laplacian_filter("laplacian_filter"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst"), _output_bmp(output_bmp)
{
	tb.i_clk(clk);
	tb.o_rst(rst);
	gaussian_filter.i_clk(clk);
	gaussian_filter.i_rst(rst);
	laplacian_filter.i_clk(clk);
	laplacian_filter.i_rst(rst);
	tb.i_gaussian(chan_gaussian);
	gaussian_filter.i_gaussian(chan_gaussian);
	gaussian_filter.i_laplacian(chan_laplacian);
	laplacian_filter.i_laplacian(chan_laplacian);
	laplacian_filter.o_result(chan_result);
	tb.o_result(chan_result);

  	tb.read_bmp(input_bmp);
}

System::~System() {
  	tb.write_bmp(_output_bmp);
}
