#include "Dut.h"

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x1.clk_rst(i_clk, i_rst);
    i_x2.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x1.reset();
		i_x2.reset();
		o_result.reset();
#endif
		wait();
	}

    enum State {
        S0, S1, S2, S3, S4, S5, S6, S7,
        S8, S9, S10, S11, S12, S13, S14, S15,
        S16, S17, S18, S19, S20, S21, S22, S23,
        S24, S25, S26, S27, S28, S29, S30, S31,
        S32, S33, S34, S35, S36, S37, S38, S39,
        S40, S41, S42, S43, S44, S45, S46, S47,
        S48, S49, S50, S51, S52, S53, S54, S55,
        S56, S57, S58, S59, S60, S61, S62, S63,
        S64, S65, S66, S67, S68, S69, S70, S71,
        S72, S73, S74, S75, S76, S77, S78, S79,
        S80, S81, S82, S83, S84, S85, S86, S87,
        S88, S89, S90, S91, S92, S93, S94, S95,
        S96, S97, S98, S99, S100, S101, S102, S103,
        S104, S105, S106, S107, S108, S109, S110, S111,
        S112, S113, S114, S115, S116, S117, S118, S119,
        S120, S121, S122, S123, S124, S125, S126, S127
    };
    State state = S0;

    while (true) {
        input_t input1, input2;

        #ifndef NATIVE_SYSTEMC
                input1 = i_x1.get();
                input2 = i_x2.get();
        #else
                input1 = i_x1.read();
                input2 = i_x2.read();
        #endif

        switch (state) {
            case S0:  
                state = (input1 && input2) ? S1 : S2; 
                break;
            case S1:  
                state = (!input1 && input2) ? S3 : S4; 
                break;
            case S2:  
                state = (input1 && !input2) ? S5 : S6; 
                break;
            case S3:  
                state = (!input1 && !input2) ? S7 : S8;
                break;
            case S4:  
                state = (input1 || input2) ? S9 : S10; 
                break;
            case S5:  
                state = (!input1 || input2) ? S11 : S12; 
                break;
            case S6:  
                state = (input1 || !input2) ? S13 : S14; 
                break;
            case S7:  
                state = (!input1 || !input2) ? S15 : S16; 
                break;
            case S8:  
                state = (input1 && input2) ? S17 : S18; 
                break;
            case S9:  
                state = (!input1 && input2) ? S19 : S20; 
                break;
            case S10: 
                state = (input1 && !input2) ? S21 : S22; 
                break;
            case S11: 
                state = (!input1 && !input2) ? S23 : S24; 
                break;
            case S12: 
                state = (input1 || input2) ? S25 : S26; 
                break;
            case S13: 
                state = (!input1 || input2) ? S27 : S28; 
                break;
            case S14: 
                state = (input1 || !input2) ? S29 : S30; 
                break;
            case S15: 
                state = (!input1 || !input2) ? S31 : S32; 
                break;
            case S16: 
                state = (input1 && input2) ? S33 : S34; 
                break;
            case S17: 
                state = (!input1 && input2) ? S35 : S36; 
                break;
            case S18: 
                state = (input1 && !input2) ? S37 : S38; 
                break;
            case S19: 
                state = (!input1 && !input2) ? S39 : S40; 
                break;
            case S20: 
                state = (input1 || input2) ? S41 : S42; 
                break;
            case S21: 
                state = (!input1 || input2) ? S43 : S44; 
                break;
            case S22: 
                state = (input1 || !input2) ? S45 : S46; 
                break;
            case S23: 
                state = (!input1 || !input2) ? S47 : S48; 
                break;
            case S24: 
                state = (input1 && input2) ? S49 : S50; 
                break;
            case S25: 
                state = (!input1 && input2) ? S51 : S52; 
                break;
            case S26: 
                state = (input1 && !input2) ? S53 : S54; 
                break;
            case S27: 
                state = (!input1 && !input2) ? S55 : S56; 
                break;
            case S28: 
                state = (input1 || input2) ? S57 : S58; 
                break;
            case S29: 
                state = (!input1 || input2) ? S59 : S60; 
                break;
            case S30: 
                state = (input1 || !input2) ? S61 : S62; 
                break;
            case S31: 
                state = (!input1 || !input2) ? S63 : S64; 
                break;
            case S32: 
                state = (input1 && input2) ? S65 : S66; 
                break;
            case S33: 
                state = (!input1 && input2) ? S67 : S68; 
                break;
            case S34: 
                state = (input1 && !input2) ? S69 : S70; 
                break;
            case S35: 
                state = (!input1 && !input2) ? S71 : S72; 
                break;
            case S36: 
                state = (input1 || input2) ? S73 : S74; 
                break;
            case S37: 
                state = (!input1 || input2) ? S75 : S76; 
                break;
            case S38: 
                state = (input1 || !input2) ? S77 : S78; 
                break;
            case S39: 
                state = (!input1 || !input2) ? S79 : S80; 
                break;
            case S40: 
                state = (input1 && input2) ? S81 : S82; 
                break;
            case S41: 
                state = (!input1 && input2) ? S83 : S84; 
                break;
            case S42: 
                state = (input1 && !input2) ? S85 : S86; 
                break;
            case S43: 
                state = (!input1 && !input2) ? S87 : S88; 
                break;
            case S44: 
                state = (input1 || input2) ? S89 : S90; 
                break;
            case S45: 
                state = (!input1 || input2) ? S91 : S92; 
                break;
            case S46: 
                state = (input1 || !input2) ? S93 : S94; 
                break;
            case S47: 
                state = (!input1 || !input2) ? S95 : S96; 
                break;
            case S48: 
                state = (input1 && input2) ? S97 : S98; 
                break;
            case S49: 
                state = (!input1 && input2) ? S99 : S100; 
                break;
            case S50: 
                state = (input1 && !input2) ? S101 : S102; 
                break;
            case S51: 
                state = (!input1 && !input2) ? S103 : S104; 
                break;
            case S52: 
                state = (input1 || input2) ? S105 : S106; 
                break;
            case S53: 
                state = (!input1 || input2) ? S107 : S108; 
                break;
            case S54: 
                state = (input1 || !input2) ? S109 : S110; 
                break;
            case S55: 
                state = (!input1 || !input2) ? S111 : S112; 
                break;
            case S56: 
                state = (input1 && input2) ? S113 : S114; 
                break;
            case S57: 
                state = (!input1 && input2) ? S115 : S116; 
                break;
            case S58: 
                state = (input1 && !input2) ? S117 : S118; 
                break;
            case S59: 
                state = (!input1 && !input2) ? S119 : S120; 
                break;
            case S60: 
                state = (input1 || input2) ? S121 : S122; 
                break;
            case S61: 
                state = (!input1 || input2) ? S123 : S124; 
                break;
            case S62: 
                state = (input1 || !input2) ? S125 : S126; 
                break;
            case S63: 
                state = (!input1 || !input2) ? S127 : S0; 
                break;
            case S64: 
                state = (input1 && input2) ? S1 : S2; 
                break;
            case S65: 
                state = (!input1 && input2) ? S3 : S4; 
                break;
            case S66: 
                state = (input1 && !input2) ? S5 : S6; 
                break;
            case S67: 
                state = (!input1 && !input2) ? S7 : S8; 
                break;
            case S68: 
                state = (input1 || input2) ? S9 : S10; 
                break;
            case S69: 
                state = (!input1 || input2) ? S11 : S12; 
                break;
            case S70: 
                state = (input1 || !input2) ? S13 : S14; 
                break;
            case S71: 
                state = (!input1 || !input2) ? S15 : S16; 
                break;
            case S72: 
                state = (input1 && input2) ? S17 : S18; 
                break;
            case S73: 
                state = (!input1 && input2) ? S19 : S20; 
                break;
            case S74: 
                state = (input1 && !input2) ? S21 : S22; 
                break;
            case S75: 
                state = (!input1 && !input2) ? S23 : S24; 
                break;
            case S76: 
                state = (input1 || input2) ? S25 : S26; 
                break;
            case S77: 
                state = (!input1 || input2) ? S27 : S28; 
                break;
            case S78: 
                state = (input1 || !input2) ? S29 : S30; 
                break;
            case S79: 
                state = (!input1 || !input2) ? S31 : S32; 
                break;
            case S80: 
                state = (input1 && input2) ? S33 : S34; 
                break;
            case S81: 
                state = (!input1 && input2) ? S35 : S36; 
                break;
            case S82: 
                state = (input1 && !input2) ? S37 : S38; 
                break;
            case S83: 
                state = (!input1 && !input2) ? S39 : S40; 
                break;
            case S84: 
                state = (input1 || input2) ? S41 : S42; 
                break;
            case S85: 
                state = (!input1 || input2) ? S43 : S44; 
                break;
            case S86: 
                state = (input1 || !input2) ? S45 : S46; 
                break;
            case S87: 
                state = (!input1 || !input2) ? S47 : S48; 
                break;
            case S88: 
                state = (input1 && input2) ? S49 : S50; 
                break;
            case S89: 
                state = (!input1 && input2) ? S51 : S52; 
                break;
            case S90: 
                state = (input1 && !input2) ? S53 : S54; 
                break;
            case S91: 
                state = (!input1 && !input2) ? S55 : S56; 
                break;
            case S92: 
                state = (input1 || input2) ? S57 : S58; 
                break;
            case S93: 
                state = (!input1 || input2) ? S59 : S60; 
                break;
            case S94: 
                state = (input1 || !input2) ? S61 : S62; 
                break;
            case S95: 
                state = (!input1 || !input2) ? S63 : S64; 
                break;
            case S96:
                state = (input1 && input2) ? S65 : S66;
                break;
            case S97: 
                state = (!input1 && input2) ? S67 : S68; 
                break;
            case S98: 
                state = (input1 && !input2) ? S69 : S70; 
                break;
            case S99: 
                state = (!input1 && !input2) ? S71 : S72; 
                break;
            case S100: 
                state = (input1 || input2) ? S73 : S74; 
                break;
            case S101: 
                state = (!input1 || input2) ? S75 : S76; 
                break;
            case S102: 
                state = (input1 || !input2) ? S77 : S78; 
                break;
            case S103: 
                state = (!input1 || !input2) ? S79 : S80; 
                break;
            case S104: 
                state = (input1 && input2) ? S81 : S82; 
                break;
            case S105: 
                state = (!input1 && input2) ? S83 : S84; 
                break;
            case S106: 
                state = (input1 && !input2) ? S85 : S86; 
                break;
            case S107: 
                state = (!input1 && !input2) ? S87 : S88; 
                break;
            case S108: 
                state = (input1 || input2) ? S89 : S90; 
                break;
            case S109: 
                state = (!input1 || input2) ? S91 : S92; 
                break;
            case S110: 
                state = (input1 || !input2) ? S93 : S94; 
                break;
            case S111: 
                state = (!input1 || !input2) ? S95 : S96; 
                break;
            case S112: 
                state = (input1 && input2) ? S97 : S98; 
                break;
            case S113: 
                state = (!input1 && input2) ? S99 : S100; 
                break;
            case S114: 
                state = (input1 && !input2) ? S101 : S102; 
                break;
            case S115: 
                state = (!input1 && !input2) ? S103 : S104; 
                break;
            case S116: 
                state = (input1 || input2) ? S105 : S106; 
                break;
            case S117: 
                state = (!input1 || input2) ? S107 : S108; 
                break;
            case S118: 
                state = (input1 || !input2) ? S109 : S110; 
                break;
            case S119: 
                state = (!input1 || !input2) ? S111 : S112; 
                break;
            case S120: 
                state = (input1 && input2) ? S113 : S114; 
                break;
            case S121: 
                state = (!input1 && input2) ? S115 : S116; 
                break;
            case S122: 
                state = (input1 && !input2) ? S117 : S118; 
                break;
            case S123: 
                state = (!input1 && !input2) ? S119 : S120; 
                break;
            case S124: 
                state = (input1 || input2) ? S121 : S122; 
                break;
            case S125: 
                state = (!input1 || input2) ? S123 : S124; 
                break;
            case S126: 
                state = (input1 || !input2) ? S125 : S126; 
                break;
            case S127: 
                state = (!input1 || !input2) ? S127 : S0; 
                break;
            default: 
                state = S0; 
                break;
        }

        output_t out_state;
        out_state = state;
        
#ifndef NATIVE_SYSTEMC
        o_result.put(out_state);
#else
        o_result.write(out_state);
#endif
    }
}
