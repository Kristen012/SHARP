#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
    i_x1.clk_rst(i_clk, i_rst);
    i_x2.clk_rst(i_clk, i_rst);
    i_x3.clk_rst(i_clk, i_rst);
    i_x4.clk_rst(i_clk, i_rst);
    o_outp.clk_rst(i_clk, i_rst);
#endif
}

void Dut::do_compute() {
    {
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
        i_x1.reset();
        i_x2.reset();
        i_x3.reset();
        i_x4.reset();
        o_outp.reset();
#endif
        wait();
    }

    enum State { INIT = 0, ANALISI_REQ = 1, ASSEGNA = 2 };
    const sc_uint<3> U1 = 0b100;
    const sc_uint<3> U2 = 0b010;
    const sc_uint<3> U3 = 0b001;
    const sc_uint<3> U4 = 0b111;

    State state = INIT;
    sc_uint<3> coda0 = 0, coda1 = 0, coda2 = 0, coda3 = 0;
    bool ru1 = false, ru2 = false, ru3 = false, ru4 = false;
    bool fu1 = false, fu2 = false, fu3 = false, fu4 = false;
    sc_uint<4> grant = 0;

    while (true) {
        switch (state) {
            case INIT:
#ifndef NATIVE_SYSTEMC
                ru1 = i_x1.get();
                ru2 = i_x2.get();
                ru3 = i_x3.get();
                ru4 = i_x4.get();
#else
                ru1 = i_x1.read();
                ru2 = i_x2.read();
                ru3 = i_x3.read();
                ru4 = i_x4.read();
#endif
                state = ANALISI_REQ;
                break;
            case ANALISI_REQ:
#ifndef NATIVE_SYSTEMC
                o_outp.put(grant);
#else
                o_outp.write(grant);
#endif
                if (ru1) {
                    if (!fu1) {
                        coda3 = coda2;
                        coda2 = coda1;
                        coda1 = coda0;
                        coda0 = U1;
                    }
                }
                else if (ru2) {
                    if (!fu2) {
                        coda3 = coda2;
                        coda2 = coda1;
                        coda1 = coda0;
                        coda0 = U2;
                    }
                }
                else if (ru3) {
                    if (!fu3) {
                        coda3 = coda2;
                        coda2 = coda1;
                        coda1 = coda0;
                        coda0 = U3;
                    }
                }
                else if (ru4) {
                    if (!fu4) {
                        coda3 = coda2;
                        coda2 = coda1;
                        coda1 = coda0;
                        coda0 = U4;
                    }
                }

                fu1 = ru1;
                fu2 = ru2;
                fu3 = ru3;
                fu4 = ru4;

                state = ASSEGNA;
                break;

            case ASSEGNA:
                if (fu1 || fu2 || fu3 || fu4) {
                    switch (coda0) {
                        case 0b100:
                            grant = 0b1000;
                            break;
                        case 0b010:
                            grant = 0b0100;
                            break;
                        case 0b001:
                            grant = 0b0010;
                            break;
                        case 0b111:
                            grant = 0b0001;
                            break;
                        default:
                            grant = 0b0000;
                            break;
                    }
                    coda0 = coda1;
                    coda1 = coda2;
                    coda2 = coda3;
                    coda3 = 0;
                }

#ifndef NATIVE_SYSTEMC
                ru1 = i_x1.get();
                ru2 = i_x2.get();
                ru3 = i_x3.get();
                ru4 = i_x4.get();
#else
                ru1 = i_x1.read();
                ru2 = i_x2.read();
                ru3 = i_x3.read();
                ru4 = i_x4.read();
#endif
                state = ANALISI_REQ;
                break;
        }

        wait();
    }
}
