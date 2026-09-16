#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <systemc>

#ifndef NATIVE_SYSTEMC
#include "cynw_fixed.h"
typedef cynw_fixed<32, 8, SC_RND, SC_SAT, 0> Float;
#else
using namespace sc_dt;
#include <sysc/datatypes/fx/sc_fixed.h>
typedef sc_fixed<32, 8, SC_RND, SC_SAT, 0> Float;
#endif

typedef Float input_t;
typedef Float output_t;
typedef Float sum_t;

const int X_SIZE = 128;

const int RESULT_SIZE = 128;

#endif
