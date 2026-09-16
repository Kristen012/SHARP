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
        for (unsigned int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            input_x[i] = i_x.get();
#else
            input_x[i] = i_x.read();
#endif
        }

        merge_sort(input_x);

        for (unsigned int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
            o_result.put(input_x[i]);
#else
            o_result.write(input_x[i]);
#endif
        }
    }
}
void Dut::merge_sort(input_t* input_x) {
    input_t tmp[X_SIZE];
	for (int width = 1; width < X_SIZE; width *= 2) {
		for (int i = 0; i < X_SIZE; i += 2 * width) {
			int left, mid, right;
			int x, y, k;

			if(i + width > X_SIZE) mid = X_SIZE;
			else mid = i + width;
			if(i + 2 * width > X_SIZE) right = X_SIZE;
			else right =  i + 2 * width;
			left = i;
			x = left;
			y = mid;
			k = left;

			while (x < mid && y < right) {
				bool take_left = false;
				take_left = (input_x[x] <= input_x[y]);
				if (take_left) {
					tmp[k] = input_x[x]; 
					x++;
				} else {
					tmp[k] = input_x[y]; 
					y++;
				}
				k++;
			}
			while (x < mid) {
				tmp[k] = input_x[x]; 
				x++; 
				k++;
			}

			while (y < right) {
				tmp[k] = input_x[y]; 
				y++; 
				k++;
			}
		}
		for (int i = 0; i < X_SIZE; ++i) {
			input_x[i] = tmp[i];
		}
	}
}
