#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#include "Defines.h"

#ifndef NATIVE_SYSTEMC
#include <esc.h>
#include <cynw_p2p.h>
#include <stratus_hls.h>
#include "Dut_Gaussian_wrap.h"
#include "Dut_Laplacian_wrap.h"
#else
#include "Dut_Gaussian.h"
#include "Dut_Laplacian.h"
#endif

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n, std::string input_bmp, std::string output_bmp );
	~System();
private:
  	Testbench tb;
#ifndef NATIVE_SYSTEMC
	Dut_Gaussian_wrapper gaussian_filter;
	Dut_Laplacian_wrapper laplacian_filter;
#else
	Dut_Gaussian gaussian_filter;
	Dut_Laplacian laplacian_filter;
#endif

	sc_clock clk;
	sc_signal<bool> rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p<input_gaussian_t> chan_gaussian;
	cynw_p2p<input_laplacian_t> chan_laplacian;
	cynw_p2p<output_t> chan_result;
#else
	sc_fifo<input_gaussian_t> chan_gaussian;
	sc_fifo<input_laplacian_t> chan_laplacian;
	sc_fifo<output_t> chan_result;
#endif

	std::string _output_bmp;
};
#endif
