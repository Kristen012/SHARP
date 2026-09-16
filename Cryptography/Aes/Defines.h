#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <systemc>
using namespace sc_dt;

#define Nb 4
#define SIZE 16
#define Nr 10
#define CIPHERNUM 11

typedef sc_uint<8> input_t;
typedef sc_uint<8> output_t;
typedef sc_uint<32> U32;
typedef sc_uint<16> U16;
typedef sc_uint<8> U8;

const int ARRAY_SIZE = 256;

#endif
