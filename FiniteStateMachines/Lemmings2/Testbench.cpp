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
	i_bump_left.reset();
	i_bump_right.reset();
	i_ground.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* l_fp = fopen("../data/input_l.dat", "r");
	if (!l_fp) {
		std::cerr << "[TB] ERROR: Could not open input_l.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < INPUT_SIZE; ++i) {
		fscanf(l_fp, "%d", &val_1);
		input_l[i] = val_1;
	}
	fclose(l_fp);

	FILE* r_fp = fopen("../data/input_r.dat", "r");
	if (!r_fp) {
		std::cerr << "[TB] ERROR: Could not open input_r.dat!" << std::endl;
		exit(1);
	}
	int val_2;
	for (int i = 0; i < INPUT_SIZE; ++i) {
		fscanf(r_fp, "%d", &val_2);
		input_r[i] = val_2;
	}
	fclose(r_fp);

	FILE* g_fp = fopen("../data/input_g.dat", "r");
	if (!g_fp) {
		std::cerr << "[TB] ERROR: Could not open input_g.dat!" << std::endl;
		exit(1);
	}
	int val_3;
	for (int i = 0; i < INPUT_SIZE; ++i) {
		fscanf(g_fp, "%d", &val_3);
		input_g[i] = val_3;
	}
	fclose(g_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < INPUT_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_bump_left.put(input_l[i]);
		i_bump_right.put(input_r[i]);
		i_ground.put(input_g[i]);
#else
		i_bump_left.write(input_l[i]);
		i_bump_right.write(input_r[i]);
		i_ground.write(input_g[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_walk_left.reset();
    o_walk_right.reset();
    o_aaah.reset();
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
		output_t inVal_left_temp = o_walk_left.get();
		output_t inVal_right_temp = o_walk_right.get();
		output_t inVal_aaah_temp = o_aaah.get();
#else
		output_t inVal_left_temp = o_walk_left.read();
		output_t inVal_right_temp = o_walk_right.read();
		output_t inVal_aaah_temp = o_aaah.read();
#endif
		int inVal_left = inVal_left_temp;
		int inVal_right = inVal_right_temp;
		int inVal_aaah = inVal_aaah_temp;
		
		fprintf( outfp, "%d %d %d\n", inVal_left, inVal_right, inVal_aaah );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
