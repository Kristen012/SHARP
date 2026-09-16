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
	i_data.reset();
	i_done_counting.reset();
	i_ack.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* data_fp = fopen("../data/input_data.dat", "r");
	if (!data_fp) {
		std::cerr << "[TB] ERROR: Could not open input_data.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(data_fp, "%d", &val_1);
		input_data[i] = val_1;
	}
	fclose(data_fp);

	FILE* done_counting_fp = fopen("../data/input_done_counting.dat", "r");
	if (!done_counting_fp) {
		std::cerr << "[TB] ERROR: Could not open input_done_counting.dat!" << std::endl;
		exit(1);
	}
	int val_2;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(done_counting_fp, "%d", &val_2);
		input_done_counting[i] = val_2;
	}
	fclose(done_counting_fp);

	FILE* ack_fp = fopen("../data/input_ack.dat", "r");
	if (!ack_fp) {
		std::cerr << "[TB] ERROR: Could not open input_ack.dat!" << std::endl;
		exit(1);
	}
	int val_3;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(ack_fp, "%d", &val_3);
		input_ack[i] = val_3;
	}
	fclose(ack_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_data.put(input_data[i]);
		i_done_counting.put(input_done_counting[i]);
		i_ack.put(input_ack[i]);
#else
		i_data.write(input_data[i]);
		i_done_counting.write(input_done_counting[i]);
		i_ack.write(input_ack[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_shift_ena.reset();
    o_counting.reset();
    o_done.reset();
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
		output_t inVal_shift_ena_temp = o_shift_ena.get();
		output_t inVal_counting_temp = o_counting.get();
		output_t inVal_done_temp = o_done.get();
#else
		output_t inVal_shift_ena_temp = o_shift_ena.read();
		output_t inVal_counting_temp = o_counting.read();
		output_t inVal_done_temp = o_done.read();
#endif
		int inVal_shift_ena = inVal_shift_ena_temp;
		int inVal_counting = inVal_counting_temp;
		int inVal_done = inVal_done_temp;
		
		fprintf( outfp, "%d %d %d\n", inVal_shift_ena, inVal_counting, inVal_done );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
