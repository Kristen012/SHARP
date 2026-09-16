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
    i_key.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* key_fp = fopen("../data/input_key.dat", "r");
	if (!key_fp) {
		std::cerr << "[TB] ERROR: Could not open input_key.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < SIZE; ++i) {
		fscanf(key_fp, "%x", &val_1);
		input_key[i] = val_1;
	}

	FILE* x_fp = fopen("../data/input_x.dat", "r");
	if (!x_fp) {
		std::cerr << "[TB] ERROR: Could not open input_x.dat!" << std::endl;
		exit(1);
	}
	int val_2;
	for (int i = 0; i < CIPHERNUM; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			fscanf(x_fp, "%x", &val_2);
			input_x[i][j] = val_2;
		}
	}
	fclose(x_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < SIZE; ++i) {
#ifndef NATIVE_SYSTEMC
		i_key.put(input_key[(i%4)*4+i/4]);
#else
		i_key.write(input_key[(i%4)*4+i/4]);
#endif
	}

	for (int i = 0; i < CIPHERNUM; ++i) {
		for (int j = 0; j < SIZE; ++j) {
#ifndef NATIVE_SYSTEMC
			i_x.put(input_x[i][(j%4)*4+j/4]);
#else
			i_x.write(input_x[i][(j%4)*4+j/4]);
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
	for (int i = 0; i < CIPHERNUM; i++) {
		for (int j = 0; j < SIZE; j++) {
#ifndef NATIVE_SYSTEMC
  			output_result[i][j] = o_result.get();
#else
  			output_result[i][j] = o_result.read();
#endif
		}
	}
	for (int i = 0; i < CIPHERNUM; i++) {
		for (int j = 0; j < SIZE; j++) {
			int inVal = output_result[i][(j%4)*4+j/4];

			if (j != SIZE - 1) {
				fprintf( outfp, "%x ", inVal );
			}
			else {
				fprintf( outfp, "%x\n", inVal );
			}
		}
	}

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
