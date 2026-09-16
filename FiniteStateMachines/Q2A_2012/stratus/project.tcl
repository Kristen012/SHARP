#*******************************************************************************
# Copyright 2015 Cadence Design Systems, Inc.
# All Rights Reserved.
#
#*******************************************************************************
#
# Stratus Project File
#
############################################################
# Project Parameters
############################################################
#
# Technology Libraries
#
set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
set LIB_LEAF "slow_vdd1v2_basicCells.lib"
use_tech_lib    "$LIB_PATH/$LIB_LEAF"

#
# Global synthesis attributes.
#
set_attr clock_period           10.0
set_attr message_detail         3 
#set_attr default_input_delay    0.1
#set_attr sched_aggressive_1 on
#set_attr unroll_loops on
#set_attr flatten_arrays all 
#set_attr timing_aggression 0
#set_attr default_protocol true

#
# Simulation Options
#
set_attr cc_options             "-DCLOCK_PERIOD=10.0 -g"
enable_waveform_logging -vcd
set_attr end_of_sim_command "make cmp_result"
set_attr rtl_annotation                 op,stack
use_systemc_simulator xcelium
use_verilog_simulator xcelium ;# 'xcelium' or 'vcs'
enable_code_coverage -verilog -html

#
# Testbench or System Level Modules
#
define_system_module ../main.cpp
define_system_module ../Testbench.cpp
define_system_module ../System.cpp

#
# SC_MODULEs to be synthesized
#
define_hls_module Dut ../Dut.cpp
define_hls_config Dut BASIC

define_sim_config B "Dut BEH BASIC"
foreach cfg { BASIC } {
	define_sim_config V_${cfg} "Dut RTL_V ${cfg}"
}

define_cov_config BASIC_COV V_BASIC \
    -imc_pro \
    -options {BDW_UNR_OPTIONS -jg_clock clk -jg_reset rst} \
      {BDW_TOP_INST_PATH sc_main.sys.dut.Dut}