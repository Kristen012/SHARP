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
	i_d.reset();
	i_done_counting.reset();
	i_ack.reset();
	i_state.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* d_fp = fopen("../data/input_d.dat", "r");
	if (!d_fp) {
		std::cerr << "[TB] ERROR: Could not open input_d.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(d_fp, "%d", &val_1);
		input_d[i] = val_1;
	}
	fclose(d_fp);

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

	FILE* state_fp = fopen("../data/input_state.dat", "r");
	if (!state_fp) {
		std::cerr << "[TB] ERROR: Could not open input_state.dat!" << std::endl;
		exit(1);
	}
	int val_4;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(state_fp, "%d", &val_4);
		input_state[i] = val_4;
	}
	fclose(state_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_d.put(input_d[i]);
		i_done_counting.put(input_done_counting[i]);
		i_ack.put(input_ack[i]);
		i_state.put(input_state[i]);
#else
		i_d.write(input_d[i]);
		i_done_counting.write(input_done_counting[i]);
		i_ack.write(input_ack[i]);
		i_state.write(input_state[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
	o_B3_next.reset();
	o_S_next.reset();
	o_S1_next.reset();
	o_Count_next.reset();
	o_Wait_next.reset();
	o_done.reset();
	o_counting.reset();
	o_shift_ena.reset();
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
		output_t inVal_B3_next_temp = o_B3_next.get();
		output_t inVal_S_next_temp = o_S_next.get();
		output_t inVal_S1_next_temp = o_S1_next.get();
		output_t inVal_Count_next_temp = o_Count_next.get();
		output_t inVal_Wait_next_temp = o_Wait_next.get();
		output_t inVal_done_temp = o_done.get();
		output_t inVal_counting_temp = o_counting.get();
		output_t inVal_shift_ena_temp = o_shift_ena.get();
#else
		output_t inVal_B3_next_temp = o_B3_next.read();
		output_t inVal_S_next_temp = o_S_next.read();
		output_t inVal_S1_next_temp = o_S1_next.read();
		output_t inVal_Count_next_temp = o_Count_next.read();
		output_t inVal_Wait_next_temp = o_Wait_next.read();
		output_t inVal_done_temp = o_done.read();
		output_t inVal_counting_temp = o_counting.read();
		output_t inVal_shift_ena_temp = o_shift_ena.read();
#endif
		int inVal_B3_next = inVal_B3_next_temp;
		int inVal_S_next = inVal_S_next_temp;
		int inVal_S1_next = inVal_S1_next_temp;
		int inVal_Count_next = inVal_Count_next_temp;
		int inVal_Wait_next = inVal_Wait_next_temp;
		int inVal_done = inVal_done_temp;
		int inVal_counting = inVal_counting_temp;
		int inVal_shift_ena = inVal_shift_ena_temp;
		
		fprintf( outfp, "%d %d %d %d %d %d %d %d\n", inVal_B3_next, inVal_S_next, inVal_S1_next, inVal_Count_next, inVal_Wait_next, inVal_done, inVal_counting, inVal_shift_ena );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
