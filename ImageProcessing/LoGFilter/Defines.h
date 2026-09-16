#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <systemc>
using namespace sc_dt;
using namespace std;

#ifndef NATIVE_SYSTEMC
typedef sc_uint<8> input_gaussian_t;
typedef sc_int<32> input_laplacian_t;
typedef sc_uint<8> output_t;
#else
typedef unsigned char input_gaussian_t;
typedef float input_laplacian_t;
typedef unsigned char output_t;
#endif

const int MASK_N = 1;
const int MASK_X = 3;
const int MASK_Y = 3;

#endif
