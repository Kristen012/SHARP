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
	i_A.reset();
    i_B.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);

	FILE* x1_fp = fopen("../data/input_A.dat", "r");
	if (!x1_fp) {
		std::cerr << "[TB] ERROR: Could not open input_A.dat!" << std::endl;
		exit(1);
	}
	int val_1;
	for (int i = 0; i < X_SIZE; i++) {
		fscanf(x1_fp, "%d", &val_1);
		A[i] = val_1;
	}
	fclose(x1_fp);

	FILE* x2_fp = fopen("../data/input_B.dat", "r");
	if (!x2_fp) {
		std::cerr << "[TB] ERROR: Could not open input_B.dat!" << std::endl;
		exit(1);
	}
	int val_2;
	for (int i = 0; i < X_SIZE; i++) {
		fscanf(x2_fp, "%d", &val_2);
		B[i] = val_2;
	}
	fclose(x2_fp);

	start_time[0] = sc_time_stamp();

	for (int i = 0; i < X_SIZE; i++) {
#ifndef NATIVE_SYSTEMC
		i_A.put(A[i]);
		i_B.put(B[i]);
#else
		i_A.write(A[i]);
		i_B.write(B[i]);
#endif
	}
}

void Testbench::do_fetch() {
#ifndef NATIVE_SYSTEMC
    o_Q.reset();
    o_R.reset();
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
		U16 inVal_Q_temp = o_Q.get();
		U16 inVal_R_temp = o_R.get();
#else
		U16 inVal_Q_temp = o_Q.read();
		U16 inVal_R_temp = o_R.read();
#endif
		int inVal_Q = inVal_Q_temp;
		int inVal_R = inVal_R_temp;
		
		fprintf( outfp, "%d %d\n", inVal_Q, inVal_R );
    }

	total_latency = (sc_time_stamp() - start_time[0]) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	fclose( outfp );
	sc_stop();
}
