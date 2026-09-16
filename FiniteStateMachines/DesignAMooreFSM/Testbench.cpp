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
	int val;
	for (int i = 0; i < S_SIZE; ++i) {
		fscanf(s_fp, "%d", &val);
		input_s[i] = val;
	}
	fclose(s_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < S_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_s.put(input_s[i]);
#else
		i_s.write(input_s[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_fr3.reset();
    o_fr2.reset();
    o_fr1.reset();
    o_dfr.reset();
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
		output_t inVal_fr3_temp = o_fr3.get();
		output_t inVal_fr2_temp = o_fr2.get();
		output_t inVal_fr1_temp = o_fr1.get();
		output_t inVal_dfr_temp = o_dfr.get();
#else
		output_t inVal_fr3_temp = o_fr3.read();
		output_t inVal_fr2_temp = o_fr2.read();
		output_t inVal_fr1_temp = o_fr1.read();
		output_t inVal_dfr_temp = o_dfr.read();
#endif
		int inVal_fr3 = inVal_fr3_temp;
		int inVal_fr2 = inVal_fr2_temp;
		int inVal_fr1 = inVal_fr1_temp;
		int inVal_dfr = inVal_dfr_temp;
		
		fprintf( outfp, "%d %d %d %d\n", inVal_fr3, inVal_fr2, inVal_fr1, inVal_dfr );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
