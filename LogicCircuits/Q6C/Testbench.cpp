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
	i_y.reset();
	i_w.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* y_fp = fopen("../data/input_y.dat", "r");
	if (!y_fp) {
		std::cerr << "[TB] ERROR: Could not open input_y.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(y_fp, "%d", &val_1);
		input_y[i] = val_1;
	}
	fclose(y_fp);

	FILE* w_fp = fopen("../data/input_w.dat", "r");
	if (!w_fp) {
		std::cerr << "[TB] ERROR: Could not open input_w.dat!" << std::endl;
		exit(1);
	}
	int val_2;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(w_fp, "%d", &val_2);
		input_w[i] = val_2;
	}
	fclose(w_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_y.put(input_y[i]);
		i_w.put(input_w[i]);
#else
		i_y.write(input_y[i]);
		i_w.write(input_w[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_Y2.reset();
    o_Y4.reset();
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
    for(int i = 0; i < RESULT_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		output_t inVal_Y2_temp = o_Y2.get();
		output_t inVal_Y4_temp = o_Y4.get();
#else
		output_t inVal_Y2_temp = o_Y2.read();
		output_t inVal_Y4_temp = o_Y4.read();
#endif
		int inVal_Y2 = inVal_Y2_temp;
		int inVal_Y4 = inVal_Y4_temp;
		
		fprintf( outfp, "%d %d\n", inVal_Y2, inVal_Y4 );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
