#include "Dut_Laplacian.h"

Dut_Laplacian::Dut_Laplacian(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_laplacian_filter);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_laplacian.clk_rst(i_clk, i_rst);
	o_result.clk_rst(i_clk, i_rst);
#endif
}

const int mask[MASK_X][MASK_Y] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

void Dut_Laplacian::do_laplacian_filter() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
        i_laplacian.reset();
        o_result.reset();
#endif
        wait();
  	}
	while (true) {
		if (counter_256 == 0) {
			for (unsigned int v = 0; v < MASK_Y; ++v) {
				for (unsigned int u = 0; u < MASK_X; ++u) {
#ifndef NATIVE_SYSTEMC
          			buffer[v][u] = i_laplacian.get();
#else
          			buffer[v][u] = i_laplacian.read();
#endif
          			wait();
        		}
      		}
    	}
		else {
			for (unsigned int v = 0; v < MASK_Y; ++v) {
				for (unsigned int u = 0; u < MASK_X - 1; ++u) {
					buffer[v][u] = buffer[v][u + 1];
					wait();
				}
			}
			for (unsigned int v = 0; v < MASK_Y; ++v) {
#ifndef NATIVE_SYSTEMC
        		buffer[v][2] = i_laplacian.get();
#else
        		buffer[v][2] = i_laplacian.read();
#endif
        		wait();
      		}
    	}
		counter_256++;
		if (counter_256 == 256) {
			counter_256 = 0;
		}
		val = 0;
    
#ifndef NATIVE_SYSTEMC
    	for (unsigned int v = 0; v < MASK_Y; ++v) {
			if (v == 1) {
				val += buffer[v][1];
			} else {
				val = val - buffer[v][1];
			}
    	}
    	for (unsigned int u = 0; u < MASK_X; ++u) {
			if (u == 1) {
				val += buffer[1][u] << 2;
			} else {
				val = val - buffer[1][u];
			}
    	}
#else
		for (unsigned int v = 0; v < MASK_Y; ++v) {
			for (unsigned int u = 0; u < MASK_X; ++u) {
				val += buffer[v][u] * mask[v][u];
			}
		}
#endif



#ifndef NATIVE_SYSTEMC
    sc_uint<32> clamp_val;
    if (val > 255 * 16) {
      	clamp_val = 255 * 16;
    }
    else if (val < 0) {
      	clamp_val = 0;
    }
    else {
      	clamp_val = sc_uint<32>(val);
    }

    bool carry_bit;
    if ((clamp_val.range(3,0) == 8 && clamp_val[4] == 1) || clamp_val.range(3,0) > 8) {
      	carry_bit = true;
    }
    else {
      	carry_bit = false;
    }

    output_t result = clamp_val / 16 + carry_bit;
    o_result.put(result);
#else
    int result;
    int int_val = (int)val;
    float frac = val - int_val;
    if (frac == 0.5) {
		if (int_val % 2 == 0) {
			result = int_val;
		}
		else {
			if (int_val >= 0) {
				result = int_val + 1;
			}
			else {
				result = int_val - 1;
			}
		}
    }
    else {
      	result = (int)round(val);
    }
    if (result > 255) {
      	result = 255;
    }
    else if (result < 0) {
      	result = 0;
    }
    o_result.write((unsigned char)result);
#endif
  }
}

Dut_Laplacian::~Dut_Laplacian() {
}