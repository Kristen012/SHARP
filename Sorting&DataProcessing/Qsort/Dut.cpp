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
        for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            input_x[i] = i_x.get();
#else
            input_x[i] = i_x.read();
#endif
        }

        sort(input_x);

        for (int i = 0; i < RESULT_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            o_result.put(input_x[i]);
#else
            o_result.write(input_x[i]);
#endif
        }
    }
}

void Dut::sort(sc_uint<8> *arr){
    int piv, beg[X_SIZE], end[X_SIZE], i=0, L, R;
    beg[0] = 0;
    end[0] = X_SIZE;

    while (i>=0) {
        L = beg[i]; R = end[i]-1;
        if (L<R) {
            piv=arr[L];
            while (L<R) {
                while (arr[R] >= piv && L < R) R--;
                    if (L < R) arr[L++]=arr[R];
                    while (arr[L] <= piv && L<R) L++;
                    if(L < R) arr[R--]=arr[L];
            }
            arr[L] = piv;
            beg[i+1] = L+1;
            end[i+1] = end[i];
            end[i++] = L;
            if (end[i] - beg[i] > end[i-1] - beg[i-1]) swap(&end[i], &beg[i]);
        } 
        else i--;
    }

}


void Dut::swap(int *end, int *beg){
    int swap;
    swap=*beg;
    *beg=*(beg-1);
    *(beg-1)=swap;

    swap=*end;
    *end=*(end-1); 
    *(end-1)=swap; 
}