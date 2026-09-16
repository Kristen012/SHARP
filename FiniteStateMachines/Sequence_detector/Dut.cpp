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

    enum State { IDLE, S1, S2, S3, S4 };
    State cur_state = IDLE;

    while (true) {
#ifndef NATIVE_SYSTEMC
        input_x = i_x.get();
#else
        input_x = i_x.read();
#endif

        switch (cur_state) {
            case IDLE:
                cur_state = input_x ? S1 : IDLE;
                break;
            case S1:
                cur_state = input_x ? S1 : S2;
                break;
            case S2:
                cur_state = input_x ? S1 : S3;
                break;
            case S3:
                cur_state = input_x ? S4 : IDLE;
                break;
            case S4:
                cur_state = input_x ? S1 : S2;
                break;
        }

        bool detected = (cur_state == S4);
        
#ifndef NATIVE_SYSTEMC
        o_result.put(detected);
#else
        o_result.write(detected);
#endif
    }
}
