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
	i_in.reset();
	i_state.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* in_fp = fopen("../data/input_in.dat", "r");
	if (!in_fp) {
		std::cerr << "[TB] ERROR: Could not open input_in.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(in_fp, "%d", &val_1);
		input_in[i] = val_1;
	}
	fclose(in_fp);

	FILE* state_fp = fopen("../data/input_state.dat", "r");
	if (!state_fp) {
		std::cerr << "[TB] ERROR: Could not open input_state.dat!" << std::endl;
		exit(1);
	}
	int val_2;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(state_fp, "%d", &val_2);
		input_state[i] = val_2;
	}
	fclose(state_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_in.put(input_in[i]);
		i_state.put(input_state[i]);
#else
		i_in.write(input_in[i]);
		i_state.write(input_state[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_out1.reset();
    o_out2.reset();
    o_state.reset();
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
		output_t inVal_out1_temp = o_out1.get();
		output_t inVal_out2_temp = o_out2.get();
		output_state_t inVal_state_temp = o_state.get();
#else
		output_t inVal_out1_temp = o_out1.read();
		output_t inVal_out2_temp = o_out2.read();
		output_state_t inVal_state_temp = o_state.read();
#endif
		int inVal_out1 = inVal_out1_temp;
		int inVal_out2 = inVal_out2_temp;
		int inVal_state = inVal_state_temp;
		
		fprintf( outfp, "%d %d %d\n", inVal_out1, inVal_out2, inVal_state );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
