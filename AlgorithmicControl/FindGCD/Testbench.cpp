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
	i_x1.reset();
    i_x2.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* x1_fp = fopen("../data/input_x1.dat", "r");
	if (!x1_fp) {
		std::cerr << "[TB] ERROR: Could not open input_x1.dat!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < X_SIZE; i++) {
		fscanf(x1_fp, "%d", &input_x1[i]);
	}
	fclose(x1_fp);

	FILE* x2_fp = fopen("../data/input_x2.dat", "r");
	if (!x2_fp) {
		std::cerr << "[TB] ERROR: Could not open input_x2.dat!" << std::endl;
		exit(1);
	}
	for (int i = 0; i < X_SIZE; i++) {
		fscanf(x2_fp, "%d", &input_x2[i]);
	}
	fclose(x2_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_x1.put(input_x1[i]);
		i_x2.put(input_x2[i]);
#else
		i_x1.write(input_x1[i]);
		i_x2.write(input_x2[i]);
#endif
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
    for(int i = 0; i < RESULT_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		output_t inVal_temp = o_result.get();
#else
		output_t inVal_temp = o_result.read();
#endif

		int inVal = inVal_temp;
		
		fprintf( outfp, "%d\n", inVal );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
