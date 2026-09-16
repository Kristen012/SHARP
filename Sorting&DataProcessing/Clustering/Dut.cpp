#include "Dut.h"

U16 safe_div(sc_uint<32> numerator, sc_uint<16> denominator) {
    return (denominator == 0) ? 0 : (numerator / denominator);
}

Dut::Dut(sc_module_name n) : sc_module(n) {
    SC_THREAD(do_compute);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_x.clk_rst(i_clk, i_rst);
    i_y.clk_rst(i_clk, i_rst);
    o_result.clk_rst(i_clk, i_rst);
#endif

}

void Dut::do_compute() {
	{
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("main_reset");
		i_x.reset();
		i_y.reset();
		o_result.reset();
#endif
		wait();
	}

    U16 centroids_x[K_VALUE];
    U16 centroids_y[K_VALUE];
	centroids_x[0] = 1000;
	centroids_y[0] = 1000;
	centroids_x[1] = 9000;
	centroids_y[1] = 1000;
	centroids_x[2] = 5000;
	centroids_y[2] = 9000;


    while (true) {
        for (int i = 0; i < DATA_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            line_x[i] = i_x.get();
            line_y[i] = i_y.get();
#else
            line_x[i] = i_x.read();
            line_y[i] = i_y.read();
#endif
        }

        bool converged = false;

        while (!converged) {
            for (int k = 0; k < K_VALUE; k++) {
                prev_x[k] = centroids_x[k];
                prev_y[k] = centroids_y[k];
            }

            for (int k = 0; k < K_VALUE; k++) {
                sum_x[k] = 0;
                sum_y[k] = 0;
                count[k] = 0;
            }

            for (int i = 0; i < DATA_SIZE; i++) {
                sc_int<32> dist[K_VALUE];
                for (int k = 0; k < K_VALUE; k++) {
                    sc_int<17> dx = line_x[i] - centroids_x[k];
                    sc_int<17> dy = line_y[i] - centroids_y[k];
                    dist[k] = dx * dx + dy * dy;
                }

                int min_idx = 0;
                if (dist[1] < dist[min_idx]) min_idx = 1;
                if (dist[2] < dist[min_idx]) min_idx = 2;

                sum_x[min_idx] += line_x[i];
                sum_y[min_idx] += line_y[i];
                count[min_idx]++;
            }

            for (int k = 0; k < K_VALUE; k++) {
                centroids_x[k] = safe_div(sum_x[k], count[k]);
                centroids_y[k] = safe_div(sum_y[k], count[k]);
            }

            converged = true;
            for (int k = 0; k < K_VALUE; k++) {
                U16 dx = (centroids_x[k] > prev_x[k]) ? (centroids_x[k] - prev_x[k]) : (prev_x[k] - centroids_x[k]);
                U16 dy = (centroids_y[k] > prev_y[k]) ? (centroids_y[k] - prev_y[k]) : (prev_y[k] - centroids_y[k]);
                if (dx > 1 || dy > 1) {
                    converged = false;
                    break;
                }
            }
        }

		U16 out_x[K_VALUE], out_y[K_VALUE];
		for (int k = 0; k < K_VALUE; k++) {
			out_x[k] = centroids_x[k];
			out_y[k] = centroids_y[k];
		}

        for (int k = 0; k < K_VALUE; k++) {
#ifndef NATIVE_SYSTEMC
			o_result.put(out_x[k]);
			o_result.put(out_y[k]);
#else
			o_result.write(out_x[k]);
			o_result.write(out_y[k]);
#endif
        }
    }
}
