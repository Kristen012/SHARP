#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
    i_x.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
    {
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
        i_x.reset();
        o_result.reset();
#endif
        wait();
    }

    enum State { s0, s1, s2, s3, s4, s5 };
    State cur_state = s0;
    State next_state = s0;

    while (true) {
#ifndef NATIVE_SYSTEMC
        input_x = i_x.get();
#else
        input_x = i_x.read();
#endif

        switch (cur_state) {
            case s0:
                next_state = input_x ? s1 : s0;
                break;
            case s1:
                next_state = input_x ? s1 : s2;
                break;
            case s2:
                next_state = input_x ? s1 : s3;
                break;
            case s3:
                next_state = input_x ? s4 : s0;
                break;
            case s4:
                next_state = input_x ? s5 : s2;
                break;
            case s5:
                next_state = input_x ? s1 : s2;
                break;
            default:
                next_state = s0;
                break;
        }

        bool match = (cur_state == s4 && input_x);
        
#ifndef NATIVE_SYSTEMC
        o_result.put(match);
#else
        o_result.write(match);
#endif

        cur_state = next_state;
    }
}
