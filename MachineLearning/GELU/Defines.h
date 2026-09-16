#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <systemc>
using namespace sc_dt;

#ifndef NATIVE_SYSTEMC
#include "cynw_float.h"
typedef cynw_float<8,23> input_t;
typedef cynw_float<8,23> output_t;
typedef cynw_float<8,23> fp32_t;
#else
typedef float input_t;
typedef float output_t;
typedef float fp32_t;
#endif

#define SIZE 128

#endif
