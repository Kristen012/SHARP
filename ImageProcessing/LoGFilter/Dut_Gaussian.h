#ifndef DUT_GAUSSIAN_H_
#define DUT_GAUSSIAN_H_

#include <systemc>
using namespace sc_core;
#include "Defines.h"
#include <cmath>

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Dut_Gaussian : public sc_module {
public:
    sc_in_clk i_clk;
    sc_in<bool> i_rst;


#ifndef NATIVE_SYSTEMC
    cynw_p2p<input_gaussian_t>::in i_gaussian;
    cynw_p2p<input_laplacian_t>::out i_laplacian;
#else
    sc_fifo_in<input_gaussian_t> i_gaussian;
    sc_fifo_out<input_laplacian_t> i_laplacian;
#endif

    SC_HAS_PROCESS(Dut_Gaussian);
    Dut_Gaussian(sc_module_name n);
    ~Dut_Gaussian();

private:
  	void do_gaussian_filter();

#ifndef NATIVE_SYSTEMC
	input_laplacian_t val;
	input_gaussian_t buffer[3][3];
#else
	input_gaussian_t buffer[3][3];
	input_laplacian_t val;
#endif
	unsigned int counter_3 = 0;
	unsigned int counter_256 = 0;
};
#endif
