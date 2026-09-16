#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"

unsigned char header[1078] = {
	0x42,         
	0x4d,         
	0,    0, 0, 0,
	0,    0,      
	0,    0,      
	0x36,   0x04, 0, 0,
	40,   0, 0, 0,
	0,    0, 0, 0,
	0,    0, 0, 0,
	1,    0,      
	8,   0,       
	0,    0, 0, 0,
	0,    0, 0, 0,
	0,    0, 0, 0,
	0,    0, 0, 0,
	0,    0, 0, 0,
	0,    0, 0, 0 
};

Testbench::Testbench(sc_module_name n)
    : sc_module(n), output_rgb_raw_data_offset(1078) {
	SC_THREAD(do_feed);
	sensitive << i_clk.pos();
	dont_initialize();
	SC_THREAD(do_fetch);
	sensitive << i_clk.pos();
	dont_initialize();

#ifndef NATIVE_SYSTEMC
	i_gaussian.clk_rst(i_clk, o_rst);     
	o_result.clk_rst(i_clk, o_rst);
#endif
}

Testbench::~Testbench() {
}

int Testbench::read_bmp(string infile_name) {
	FILE *fp_s = NULL;
	fp_s = fopen(infile_name.c_str(), "rb");
	if (fp_s == NULL) {
		printf("fopen %s error\n", infile_name.c_str());
		return -1;
	}

	fseek(fp_s, 10, SEEK_SET);
	assert(fread(&input_rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s));

	fseek(fp_s, 18, SEEK_SET);
	assert(fread(&width, sizeof(unsigned int), 1, fp_s));
	assert(fread(&height, sizeof(unsigned int), 1, fp_s));

	fseek(fp_s, 28, SEEK_SET);
	assert(fread(&bits_per_pixel, sizeof(unsigned short), 1, fp_s));
	bytes_per_pixel = bits_per_pixel / 8;
	printf("pixel_size: %d\n", bits_per_pixel);

  	fseek(fp_s, input_rgb_raw_data_offset, SEEK_SET);

	source_bitmap =
		(unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
	if (source_bitmap == NULL) {
		printf("malloc images_s error\n");
		return -1;
	}

	target_bitmap =
		(unsigned char *)malloc((size_t)width * height * bytes_per_pixel);
	if (target_bitmap == NULL) {
		printf("malloc target_bitmap error\n");
		return -1;
	}

	printf("Image width=%d, height=%d\n", width, height);
	assert(fread(source_bitmap, sizeof(unsigned char),
				(size_t)(long)width * height * bytes_per_pixel, fp_s));
	fclose(fp_s);
	return 0;
}

int Testbench::write_bmp(string outfile_name) {
	FILE *fp_t = NULL;
	unsigned int file_size;

	fp_t = fopen(outfile_name.c_str(), "wb");
	if (fp_t == NULL) {
		printf("fopen %s error\n", outfile_name.c_str());
		return -1;
	}

	file_size = width * height * bytes_per_pixel + output_rgb_raw_data_offset;
	header[2] = (unsigned char)(file_size & 0x000000ff);
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;

	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) & 0x000000ff;
	header[20] = (width >> 16) & 0x000000ff;
	header[21] = (width >> 24) & 0x000000ff;

	header[22] = height & 0x000000ff;
	header[23] = (height >> 8) & 0x000000ff;
	header[24] = (height >> 16) & 0x000000ff;
	header[25] = (height >> 24) & 0x000000ff;

  	header[28] = bits_per_pixel;

	for (int i = 0; i < 256; i++) {
		memset(&header[54 + i * 4], i, 3), header[54 + i * 4 + 3] = 0;
	}

  	fwrite(header, sizeof(unsigned char), output_rgb_raw_data_offset, fp_t);

	fwrite(target_bitmap, sizeof(unsigned char),
			(size_t)(long)width * height * bytes_per_pixel, fp_t);

	fclose(fp_t);
	return 0;
}

void Testbench::do_feed() {
	unsigned int x, y, v, u, vv, uu;
	unsigned char Grey;
	unsigned int adjustX, adjustY, xBound, yBound;
	unsigned int total;
	unsigned int fifo_write_cnt = 0;

#ifndef NATIVE_SYSTEMC
	i_gaussian.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();

	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			adjustX = (MASK_X % 2) ? 1 : 0;
			adjustY = (MASK_Y % 2) ? 1 : 0;
			xBound = MASK_X / 2;
			yBound = MASK_Y / 2;
		
			if (counter_256 == 0) {
				for (v = -yBound; v != yBound + adjustY; ++v) {
					for (u = -xBound; u != xBound + adjustX; ++u) {
						if (counter_3 == 0 || counter_3 == 1) {
							for (vv = -yBound; vv != yBound + adjustY; ++vv) {
								for (uu = -xBound; uu != xBound + adjustX; ++uu) {
									if (x + u < 0 || x + u >= width || y + v < 0 || y + v >= height) {
										Grey = 0;
									}
									else {
										if (x + u + uu >= 0 && x + u + uu < width && y + v + vv >= 0 && y + v + vv < height) {
											Grey = *(source_bitmap +
												bytes_per_pixel * (width * (y + v + vv) + (x + u + uu)));
										} else {
											Grey = 0;
										}
									}
									fifo_write_cnt++;
#ifndef NATIVE_SYSTEMC
									i_gaussian.put(Grey);
#else
									i_gaussian.write(Grey);
#endif
								}
							}
						}
						else {
							for (vv = -yBound; vv != yBound + adjustY; ++vv) {
								if (x + u < 0 || x + u >= width || y + v < 0 || y + v >= height) {
									Grey = 0;
								}
								else {
									if (x + u + 1 >= 0 && x + u + 1 < width && y + v + vv >= 0 && y + v + vv < height) {
										Grey = *(source_bitmap +
											bytes_per_pixel * (width * (y + v + vv) + (x + u + 1)));
									} else {
										Grey = 0;
									}
								}
								fifo_write_cnt++;
#ifndef NATIVE_SYSTEMC
								i_gaussian.put(Grey);
#else
								i_gaussian.write(Grey);
#endif
							}
						}
						counter_3++;
						if (counter_3 == 3) {
							counter_3 = 0;
						}
					}
				}
			}
			else {
				for (v = -yBound; v != yBound + adjustY; ++v) {
					for (vv = -yBound; vv != yBound + adjustY; ++vv) {
						for (uu = -xBound; uu != xBound + adjustX; ++uu) {
							if (x + 1 < 0 || x + 1 >= width || y + v < 0 || y + v >= height) {
								Grey = 0;
							}
							else {
								if (x + 1 + uu >= 0 && x + 1 + uu < width && y + v + vv >= 0 && y + v + vv < height) {
									Grey = *(source_bitmap +
											bytes_per_pixel * (width * (y + v + vv) + (x + 1 + uu)));
								} else {
									Grey = 0;
								}
							}
							fifo_write_cnt++;
#ifndef NATIVE_SYSTEMC
							i_gaussian.put(Grey);
#else
							i_gaussian.write(Grey);
#endif
						}
					}
				}
			}
			counter_256++;
			if (counter_256 == 256) {
				counter_256 = 0;
			}
		}
	}
}

void Testbench::do_fetch() {
	unsigned int x, y;
	output_t total;
#ifndef NATIVE_SYSTEMC
  	o_result.reset();
#endif
	wait(5);
	wait(1);

	sc_clock * clk_p = dynamic_cast < sc_clock * >( i_clk.get_interface() );
	sc_time clock_period = clk_p->period();

	unsigned long total_latency = 0;
	for (y = 0; y != height; ++y) {
		for (x = 0; x != width; ++x) {
#ifndef NATIVE_SYSTEMC
			total = o_result.get();
#else
			total = o_result.read();
#endif
      		*(target_bitmap + (width * y + x)) = (unsigned char)total;
    	}
	}
	total_latency = (sc_time_stamp() - total_start_time) / clock_period;
	std::cout << "Total latency " << total_latency << " cycles." << std::endl;

	sc_stop();
}