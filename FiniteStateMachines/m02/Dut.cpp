#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
    i_x.clk_rst(i_clk, i_rst);
    o_outp.clk_rst(i_clk, i_rst);
#endif
}

void Dut::do_compute() {
    {
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
        i_x.reset();
        o_outp.reset();
#endif
        wait();
    }

    enum State {
        A = 0,
        B = 1,
        C = 2,
        D = 3,
        E = 4,
        F = 5,
        G = 6
    };

    State state = A;

    while (true) {
        input_t LINEA;
        output_t U = 0;

#ifndef NATIVE_SYSTEMC
        LINEA = i_x.get();
#else
        LINEA = i_x.read();
#endif

        switch (state) {
            case A:
                state = B;
                U = 0;
                break;
            case B:
                if (LINEA == 0)
                    state = C;
                else
                    state = F;
                U = 0;
                break;
            case C:
                if (LINEA == 0)
                    state = D;
                else
                    state = G;
                U = 0;
                break;
            case D:
                state = E;
                U = 0;
                break;
            case E:
                state = B;
                U = 1;
                break;
            case F:
                state = G;
                U = 0;
                break;
            case G:
                if (LINEA == 0)
                    state = E;
                else
                    state = A;
                U = 0;
                break;
        }

#ifndef NATIVE_SYSTEMC
        o_outp.put(U);
#else
        o_outp.write(U);
#endif
        wait();
    }
}
