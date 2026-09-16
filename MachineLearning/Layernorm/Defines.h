#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <systemc>
using namespace sc_dt;

#ifndef NATIVE_SYSTEMC
#include "cynw_float.h"
typedef cynw_float<8,23> Float;
#else
typedef float Float;
#endif
typedef Float input_t;
typedef Float output_t;
typedef Float sum_t;

const int X_SIZE = 128;

const int RESULT_SIZE = 128;

#endif
