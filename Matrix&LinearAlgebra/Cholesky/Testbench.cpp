#include "Testbench.h"
#ifndef NATIVE_SYSTEMC[
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
	o_x1.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* x1_fp = fopen("../data/input_x.dat", "r");
	if (!x1_fp) {
		std::cerr << "[TB] ERROR: Could not open ../data/input_x.dat" << std::endl;
		exit(1);
	}
	float val_1;
	for (int i = 0; i < MATRIX_SIZE; i++) {
		fscanf(x1_fp, "%f", &val_1);
        in_data[i] = val_1;
	}
	fclose(x1_fp);

	FILE* golden_fp = fopen("../data/golden.dat", "r");
	if (!golden_fp) {
		std::cerr << "[TB] ERROR: Could not open ../data/golden.dat" << std::endl;
		exit(1);
	}
	for (int i = 0; i < MATRIX_SIZE; i++) {
		fscanf(golden_fp, "%f", &val_1);
        golden[i] = val_1;
	}
	fclose(golden_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < MATRIX_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		o_x1.put(in_data[i]);
#else
		o_x1.write(in_data[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    i_result.reset();
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
    for(int i = 0; i < MATRIX_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		output_t inVal_temp = i_result.get();
#else
		output_t inVal_temp = i_result.read();
#endif

		float inVal = (float) inVal_temp;
        
		fprintf( outfp, "%f\n", inVal );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << "." << std::endl;

	if (success) {
		std::cout << "===== Simulation result matches golden reference. =====" << std::endl;
	} else {
		std::cout << "===== Simulation result has mismatches! =====" << std::endl;
	}

	fclose( outfp );
	sc_stop();
}
