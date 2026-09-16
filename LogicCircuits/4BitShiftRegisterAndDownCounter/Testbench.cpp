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
	i_s.reset();
	i_c.reset();
	i_d.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* s_fp = fopen("../data/input_s.dat", "r");
	if (!s_fp) {
		std::cerr << "[TB] ERROR: Could not open input_s.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(s_fp, "%d", &val_1);
		input_s[i] = val_1;
	}
	fclose(s_fp);

	FILE* c_fp = fopen("../data/input_c.dat", "r");
	if (!c_fp) {
		std::cerr << "[TB] ERROR: Could not open input_c.dat!" << std::endl;
		exit(1);
	}
	int val_2;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(c_fp, "%d", &val_2);
		input_c[i] = val_2;
	}
	fclose(c_fp);

	FILE* d_fp = fopen("../data/input_d.dat", "r");
	if (!d_fp) {
		std::cerr << "[TB] ERROR: Could not open input_d.dat!" << std::endl;
		exit(1);
	}
	int val_3;
	for (int i = 0; i < X_SIZE; ++i) {
		fscanf(d_fp, "%d", &val_3);
		input_d[i] = val_3;
	}
	fclose(d_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_s.put(input_s[i]);
		i_c.put(input_c[i]);
		i_d.put(input_d[i]);
#else
		i_s.write(input_s[i]);
		i_c.write(input_c[i]);
		i_d.write(input_d[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_q.reset();
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
		output_t inVal_q_temp = o_q.get();
#else
		output_t inVal_q_temp = o_q.read();
#endif
		int inVal_q = inVal_q_temp;
		
		fprintf( outfp, "%d\n", inVal_q );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
