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

    while (true) {
        for(int i = 0; i < X_SIZE; i++){
#ifndef NATIVE_SYSTEMC
            input_x[i] = i_x.get();
#else
            input_x[i] = i_x.read();
#endif
        }
        sum = 0;
        for(int i = 0; i < X_SIZE; i++){
            sum = sum + input_x[i];
        }
        Float mean, dev;
        mean = sum / X_SIZE;

        sumsq = 0;
        for(int i = 0; i < X_SIZE; i++){
            sumsq = sumsq + (input_x[i] - mean) * (input_x[i] - mean);
        }
        dev = sqrt(sumsq / X_SIZE + 0.00001);
        if(dev == 0) std::cout << "dev: " << dev << "\n";
        
        for(int i = 0; i < X_SIZE; i++){
            result[i] = (input_x[i] - mean) / dev;
        }

        for (int i = 0; i < RESULT_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            o_result.put(result[i]);
#else
            o_result.write(result[i]);
#endif
        }
    }
}
