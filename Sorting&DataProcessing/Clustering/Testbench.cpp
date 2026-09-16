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
    i_y.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* points_fp = fopen("../data/input_points.dat", "r");
	if (!points_fp) {
		std::cerr << "[TB] ERROR: Could not open input_points.dat!" << std::endl;
		exit(1);
	}
	int val_x, val_y;
	for (int i = 0; i < DATA_SIZE; i++) {
		if (fscanf(points_fp, "%d, %d", &val_x, &val_y) != 2) {
			std::cerr << "[TB] ERROR: Invalid data format at line " << i << std::endl;
			exit(1);
		}
		input_xy[i][0] = val_x;
		input_xy[i][1] = val_y;
	}
	fclose(points_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < DATA_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_x.put(input_xy[i][0]);
		i_y.put(input_xy[i][1]);
#else
		i_x.write(input_xy[i][0]);
		i_y.write(input_xy[i][1]);
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
    for(int i = 0; i < K_VALUE; i++) {
#ifndef NATIVE_SYSTEMC
		output_t inVal_x_temp = o_result.get();
		output_t inVal_y_temp = o_result.get();
#else
		output_t inVal_x_temp = o_result.read();
		output_t inVal_y_temp = o_result.read();
#endif

		int inVal_x = inVal_x_temp;
		int inVal_y = inVal_y_temp;
		
		fprintf( outfp, "%d, %d\n", inVal_x, inVal_y );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
