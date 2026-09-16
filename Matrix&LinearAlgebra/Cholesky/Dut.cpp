#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x1.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x1.reset();
		o_result.reset();
#endif
		wait();
	}

    while (true) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            line_x1[i] = i_x1.get();
#else
            line_x1[i] = i_x1.read();
#endif
        }

        cholesky_compute(line_x1);

        for (int i = 0; i < MATRIX_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            o_result.put(result[i]);
#else
            o_result.write(result[i]);
#endif
        }
    }
}
void Dut::cholesky_compute(input_t m1[MATRIX_SIZE]){
    unsigned int i, j, k;
    unsigned int n = MATRIX_DIM;
    input_t diag = 0;
    input_t s = 0;
    for (unsigned i = 0; i < n; ++i)
        for (unsigned j = i+1; j < n; ++j)
            result[i*n + j] = (input_t) 0;

    for (i = 0; i < n; i++){
        for (j = 0; j < (i+1); j++) {
            s = 0;
            for (k = 0; k < j; k++) s += result[i * n + k] * result[j * n + k];
            if(i == j) {
                diag = m1[i * n + i] - s;
                result[(i * n) + j] = sqrt(diag);
            }	
            else {
                if( result[j * n + j] != 0) result[i * n + j] = (1.0 / result[(j * n) + j] * (m1[(i * n) + j] - s));
                else result[i * n + j] = 0;	 

            }
        }
    }
}

