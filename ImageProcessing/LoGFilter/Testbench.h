#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <systemc>
using namespace sc_core;
#include "Defines.h"
#include <string>

#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

#define WHITE = 255
#define BLACK = 0
#define THRESHOLD = 90

class Testbench : public sc_module {
public:
	sc_in_clk i_clk;
	sc_out<bool> o_rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p<input_gaussian_t>::base_out i_gaussian;
	cynw_p2p<output_t>::base_in o_result;
#else
	sc_fifo_out<input_gaussian_t> i_gaussian;
	sc_fifo_in<output_t> o_result;
#endif

	SC_HAS_PROCESS(Testbench);

	Testbench(sc_module_name n);
	~Testbench();

	int read_bmp(string infile_name);
	int write_bmp(string outfile_name);

	unsigned int get_width() { return width; }

	unsigned int get_height() { return height; }

	unsigned int get_width_bytes() { return width_bytes; }

	unsigned int get_bytes_per_pixel() { return bytes_per_pixel; }

	unsigned char *get_source_image() { return source_bitmap; }
	unsigned char *get_target_image() { return target_bitmap; }

private:
	unsigned int input_rgb_raw_data_offset;
	const unsigned int output_rgb_raw_data_offset;
	unsigned int width;
	unsigned int height;
	unsigned int width_bytes;
	unsigned char bits_per_pixel;
	unsigned short bytes_per_pixel;
	unsigned char *source_bitmap;
	unsigned char *target_bitmap;
	
	sc_time total_start_time;
	sc_time total_run_time;

	unsigned int counter_3 = 0;
	unsigned int counter_256 = 0;

	void do_feed();
	void do_fetch();
};
#endif
