#ifndef DUT_H_
#define DUT_H_

#include <systemc>
using namespace sc_core;
#include "Defines.h"

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

class Dut : public sc_module {
public:
	sc_in_clk i_clk;
	sc_in<bool> i_rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p<input_t>::in i_x;
  	cynw_p2p<input_t>::in i_key;
	cynw_p2p<output_t>::out o_result;
#else
	sc_fifo_in<input_t> i_x;
	sc_fifo_in<input_t> i_key;
	sc_fifo_out<output_t> o_result;
#endif

	SC_HAS_PROCESS(Dut);
	Dut(sc_module_name n);
	~Dut() = default;

private:

  	void do_compute();
    void AddRoundKey(U8 state[SIZE], U8 key[SIZE]);
    void SubBytes(U8 state[SIZE]);
    void ShiftRows(U8 state[SIZE]);
    void MixColumns(U8 state[SIZE]);
    void KeyExpansion(U8 current_key[SIZE], U8 rcon, U8 next_key[SIZE]);
    void assign_key(U8 key_in[SIZE], U8 key_out[SIZE]);
    U8 xtime_2(U8 data);
    U8 xtime_3(U8 data);
    U8 xadd(U8 in1, U8 in2);
    unsigned int clip(unsigned int n);

};
#endif
