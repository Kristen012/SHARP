#include "Dut_Gaussian.h"

Dut_Gaussian::Dut_Gaussian(sc_module_name n) : sc_module(n) {
	SC_THREAD(do_gaussian_filter);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_gaussian.clk_rst(i_clk, i_rst);
	i_laplacian.clk_rst(i_clk, i_rst);
#endif
}

#ifndef NATIVE_SYSTEMC
  	const int mask[MASK_X][MASK_Y] = {{0, 1, 0}, {1, 2, 1}, {0, 1, 0}};
#else
  	const int mask[MASK_X][MASK_Y] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
#endif

void Dut_Gaussian::do_gaussian_filter() {
  	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_gaussian.reset();
		i_laplacian.reset();
#endif
    	wait();
  	}

	while (true) {
		if (counter_256 == 0) {
			for (unsigned int k = 0; k < MASK_Y * MASK_X; ++k) {
				if (counter_3 == 0 || counter_3 == 1) {
					for (unsigned int v = 0; v < MASK_Y; ++v) {
						for (unsigned int u = 0; u < MASK_X; ++u) {
#ifndef NATIVE_SYSTEMC
              				buffer[v][u] = i_gaussian.get();
#else
              				buffer[v][u] = i_gaussian.read();
#endif
              				wait();
            			}
          			}
        		} else {
          			for (unsigned int v = 0; v < MASK_Y; ++v) {
						for (unsigned int u = 0; u < MASK_X - 1; ++u) {
							buffer[v][u] = buffer[v][u + 1];
							wait();
						}
          			}
					for (unsigned int v = 0; v < MASK_Y; ++v) {
#ifndef NATIVE_SYSTEMC
						buffer[v][2] = i_gaussian.get();
#else
						buffer[v][2] = i_gaussian.read();
#endif
						wait();
					}
        		}

				counter_3++;
				if (counter_3 == 3) counter_3 = 0;

				val = 0;
				for (unsigned int v = 0; v < MASK_Y; ++v) {
					for (unsigned int u = 0; u < MASK_X; ++u) {
#ifndef NATIVE_SYSTEMC
            			val += buffer[v][u] << mask[v][u];
#else
            			val += buffer[v][u] * mask[v][u];
#endif
          			}
        		}

#ifndef NATIVE_SYSTEMC
				input_laplacian_t result = val;
				i_laplacian.put(result);
#else
				input_laplacian_t result = val / 16.0;
				i_laplacian.write(result);
#endif
      		}
		} else {
			for (unsigned int k = 0; k < MASK_Y; ++k) {
				for (unsigned int v = 0; v < MASK_Y; ++v) {
					for (unsigned int u = 0; u < MASK_X; ++u) {
#ifndef NATIVE_SYSTEMC
            			buffer[v][u] = i_gaussian.get();
#else
            			buffer[v][u] = i_gaussian.read();
#endif
            			wait();
          			}
        		}

				val = 0;
				for (unsigned int v = 0; v < MASK_Y; ++v) {
          			for (unsigned int u = 0; u < MASK_X; ++u) {
#ifndef NATIVE_SYSTEMC
            			val += buffer[v][u] << mask[v][u];
#else
            			val += buffer[v][u] * mask[v][u];
#endif
          			}
        		}

#ifndef NATIVE_SYSTEMC
				input_laplacian_t result = val;
				i_laplacian.put(result);
#else
				input_laplacian_t result = val / 16.0;
				i_laplacian.write(result);
#endif
			}
		}

		counter_256++;
		if (counter_256 == 256) counter_256 = 0;
  	}
}

Dut_Gaussian::~Dut_Gaussian() {
}
