#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include <esc.h>
#endif
#include <string>
#include <iostream>

Testbench::Testbench(sc_module_name n)
    : sc_module(n) {
    SC_THREAD(do_feed);
    sensitive << i_clk.pos();
    dont_initialize();
    SC_THREAD(do_fetch);
    sensitive << i_clk.pos();
    dont_initialize();
}

void Testbench::do_feed() {

#ifndef NATIVE_SYSTEMC
	i_x.reset();
    i_w.reset();
    i_idx.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* w_fp = fopen("../data/input_w.dat", "r");
	if (!w_fp) {
		std::cerr << "[TB] ERROR: Could not open input_x1.dat!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < MAT_W_ROW_SIZE; i++) {
		for (int j = 0; j < MAT_W_COL_SIZE; j++) {
			fscanf(w_fp, "%d", &input_w[i][j]);
		} 
	}
	fclose(w_fp);

	FILE* x_fp = fopen("../data/input_x.dat", "r");
	if (!x_fp) {
		std::cerr << "[TB] ERROR: Could not open input_x2.dat!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < MAT_X_ROW_SIZE; i++) {
		for (int j = 0; j < MAT_X_COL_SIZE; j++) {
			fscanf(x_fp, "%d", &input_x[i][j]);
		}
	}
	fclose(x_fp);

	start_time[0] = sc_time_stamp();

	for (int idx = 0; idx < MAT_X_ROW_SIZE; idx++) {
#ifndef NATIVE_SYSTEMC
		i_idx.put(idx);
#else
		i_idx.write(idx);
#endif
		for (int i = 0; i < MAT_W_ROW_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
			i_w.put(input_w[i][idx]);
#else
			i_w.write(input_w[i][idx]);
#endif
		}
		for (int i = 0; i < MAT_X_COL_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
			i_x.put(input_x[idx][i]);
#else
			i_x.write(input_x[idx][i]);
#endif
		}
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_result.reset();
#endif
    wait(5);
	wait(1);

	sc_clock * clk_p = dynamic_cast < sc_clock * >( i_clk.get_interface() );
	sc_time clock_period = clk_p->period();

	char output_file[256];
#ifndef NATIVE_SYSTEMC
	sprintf( output_file, "%s/response.dat", getenv("BDW_SIM_CONFIG_DIR") );
#else
	sprintf( output_file, "response.dat" );
#endif
	FILE* outfp = fopen(output_file, "wb");
	if (outfp == NULL)
	{
		std::cout << "Couldn't open output.dat for writing." << std::endl;
		exit(0);
	}

	unsigned long total_latency = 0;
	bool success = true;
    for(int i = 0; i < MAT_R_ROW_SIZE; i++) {
        for(int j = 0; j < MAT_R_COL_SIZE; j++) {
#ifndef NATIVE_SYSTEMC
			output_t inVal = o_result.get();
#else
			output_t inVal = o_result.read();
#endif
			
			fprintf( outfp, "%d\n", inVal );
        }
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
