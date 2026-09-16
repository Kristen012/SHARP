#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
    i_x1.clk_rst(i_clk, i_rst);
    i_x2.clk_rst(i_clk, i_rst);
    o_outp.clk_rst(i_clk, i_rst);
    o_overflw.clk_rst(i_clk, i_rst);
#endif
}

void Dut::do_compute() {
    {
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
        i_x1.reset();
        i_x2.reset();
        o_outp.reset();
        o_overflw.reset();
#endif
        wait();
    }

    enum State {
        a = 0,
        b = 1,
        c = 2,
        e = 3,
        f = 4,
        g = 5,
        wf0 = 6,
        wf1 = 7
    };

    State state = a;

    while (true) {
        input_t LINE1, LINE2;
        output_t OUTP = 0;
        output_t OVERFLW = 0;

#ifndef NATIVE_SYSTEMC
        LINE1 = i_x1.get();
        LINE2 = i_x2.get();
#else
        LINE1 = i_x1.read();
        LINE2 = i_x2.read();
#endif

        switch (state) {
            case a:
                if (LINE1 && LINE2)
                    state = f;
                else
                    state = b;
                OUTP = LINE1 ^ LINE2;
                OVERFLW = 0;
                break;
            case e:
                if (LINE1 && LINE2)
                    state = f;
                else
                    state = b;
                OUTP = LINE1 ^ LINE2;
                OVERFLW = 1;
                break;
            case b:
                if (LINE1 && LINE2)
                    state = g;
                else
                    state = c;
                OUTP = LINE1 ^ LINE2;
                OVERFLW = 0;
                break;
            case f:
                if (LINE1 || LINE2)
                    state = g;
                else
                    state = c;
                OUTP = !(LINE1 ^ LINE2);
                OVERFLW = 0;
                break;
            case c:
                if (LINE1 && LINE2)
                    state = wf1;
                else
                    state = wf0;
                OUTP = LINE1 ^ LINE2;
                OVERFLW = 0;
                break;
            case g:
                if (LINE1 || LINE2)
                    state = wf1;
                else
                    state = wf0;
                OUTP = !(LINE1 ^ LINE2);
                OVERFLW = 0;
                break;
            case wf0:
                if (LINE1 && LINE2)
                    state = e;
                else
                    state = a;
                OUTP = LINE1 ^ LINE2;
                OVERFLW = 0;
                break;
            case wf1:
                if (LINE1 || LINE2)
                    state = e;
                else
                    state = a;
                OUTP = !(LINE1 ^ LINE2);
                OVERFLW = 0;
                break;
        }

#ifndef NATIVE_SYSTEMC
        o_outp.put(OUTP);
        o_overflw.put(OVERFLW);
#else
        o_outp.write(OUTP);
        o_overflw.write(OVERFLW);
#endif
        wait();
    }
}
