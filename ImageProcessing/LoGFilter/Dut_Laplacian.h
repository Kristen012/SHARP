#ifndef DUT_LAPLACIAN_H_
#define DUT_LAPLACIAN_H_

#include <systemc>
using namespace sc_core;
#include "Defines.h"
#include <cmath>

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Dut_Laplacian : public sc_module {
public:
    sc_in_clk i_clk;
    sc_in<bool> i_rst;

#ifndef NATIVE_SYSTEMC
    cynw_p2p<input_laplacian_t>::in i_laplacian;
    cynw_p2p<output_t>::out o_result;
#else
    sc_fifo_in<input_laplacian_t> i_laplacian;
    sc_fifo_out<output_t> o_result;
#endif

    SC_HAS_PROCESS(Dut_Laplacian);
    Dut_Laplacian(sc_module_name n);
    ~Dut_Laplacian();

private:
  void do_laplacian_filter();
#ifndef NATIVE_SYSTEMC
    input_laplacian_t buffer[3][3];
    input_laplacian_t val;
#else
	input_laplacian_t buffer[3][3];
    input_laplacian_t val;
#endif
	unsigned int counter_256 = 0;
};
#endif
