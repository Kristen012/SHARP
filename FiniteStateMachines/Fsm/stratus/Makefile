###############################################################################
#
# Copyright (c) 2019 Cadence Design Systems, Inc. All rights reserved worldwide.
#
# The code contained herein is the proprietary and confidential information
# of Cadence or its licensors, and is supplied subject to a previously
# executed license and maintenance agreement between Cadence and customer.
# This code is intended for use with Cadence high-level synthesis tools and
# may not be used with other high-level synthesis tools. Permission is only
# granted to distribute the code as indicated. Cadence grants permission for
# customer to distribute a copy of this code to any partner to aid in designing
# or verifying the customer's intellectual property, as long as such
# distribution includes a restriction of no additional distributions from the
# partner, unless the partner receives permission directly from Cadence.
#
# ALL CODE FURNISHED BY CADENCE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
# KIND, AND CADENCE SPECIFICALLY DISCLAIMS ANY WARRANTY OF NONINFRINGEMENT,
# FITNESS FOR A PARTICULAR PURPOSE OR MERCHANTABILITY. CADENCE SHALL NOT BE
# LIABLE FOR ANY COSTS OF PROCUREMENT OF SUBSTITUTES, LOSS OF PROFITS,
# INTERRUPTION OF BUSINESS, OR FOR ANY OTHER SPECIAL, CONSEQUENTIAL OR
# INCIDENTAL DAMAGES, HOWEVER CAUSED, WHETHER FOR BREACH OF WARRANTY,
# CONTRACT, TORT, NEGLIGENCE, STRICT LIABILITY OR OTHERWISE.
#
################################################################################

# Include the Makefile.prj file extracted from project.tcl
-include Makefile.prj

# Rule to update Makefile.prj anytime project.tcl changes
Makefile.prj : project.tcl
	@bdw_makegen

# List subdirectories that contain memory libraries
MEMLIBS =

# List subdirectories that contain interface libraries
IFLIBS =

# The cmp_result target compares the golden results file (golden.dat) with
# the simulation results file (data.out) written by the testbench into the 
# BDW_SIM_CONFIG simulation configuration directory. This is run automatically
# at the end of each simulation (endOfSimCommand in project.tcl).
GOLD_FILE = ../data/golden.dat
RESULTS_FILE = $(BDW_SIM_CONFIG_DIR)/response.dat

cmp_result:
	@if [ -f $(RESULTS_FILE) ] ; then \
	  echo "****************************************" ; \
	  echo `date` ; \
	  echo "Performing Simulation Results Comparison" ; \
	  echo "for $(BDW_SIM_CONFIG) Simulation..." ; \
	  if diff $(RESULTS_FILE) $(GOLD_FILE) ; then \
	    echo "  $(BDW_SIM_CONFIG): SIMULATION PASSED" ; \
	    bdw_sim_pass ; \
	  else \
	    echo "  $(BDW_SIM_CONFIG): SIMULATION FAILED" ; \
	    bdw_sim_fail ; \
	  fi ; \
	  echo "****************************************" ; \
	else \
	  echo "****************************************" ; \
	  echo `date` ; \
	  echo "ERROR: Simulation output doesn't exist" ; \
	  echo "for $(BDW_SIM_CONFIG) Simulation..." ; \
	  echo "  $(BDW_SIM_CONFIG): SIMULATION FAILED" ; \
	  bdw_sim_fail ; \
	  echo "****************************************" ; \
	fi


# Clean out undesirable junk files from the project directory
# Uses the automatically created clean_all target from Makefile.prj
clean: clean_all
	@rm -rf bdw_work vsim* *.wlf data.out *~ *.bak *.BAK Makefile.prj
	@rm -rf core *.pro *.user *.log .*.qws .*.check .stack* .parts*
	@rm -rf rc.* fv .stratus*

CLEAN: clean

COV_PATH_PRI = bdw_work/cov/BASIC_COV/cov_work/scope/__jg_coverage_unr_merge_tmp_unr
COV_PATH_SEC = bdw_work/cov/BASIC_COV/cov_work/scope/final_merge

RPT_CMD = report -detail -text -out ../coverage_report.log; exit

run_uc: unr_BASIC_COV
	@echo "--------------------------------------------------"
	@echo "Checking for UNR coverage directory..."
	@found=0; \
	for i in $$(seq 1 20); do \
		if [ -d "$(COV_PATH_PRI)" ]; then \
			echo "Directory found after $$i seconds."; \
			found=1; \
			break; \
		fi; \
		echo "Waiting for filesystem sync... ($$i/20)"; \
		sleep 1; \
	done; \
	\
	if [ $$found -eq 1 ]; then \
		echo "Generating IMC Report from Primary Path..."; \
		imc -load $(COV_PATH_PRI) -execcmd "$(RPT_CMD)"; \
	else \
		echo "Warning: Primary path timed out. Trying fallback path: final_merge..."; \
		imc -load $(COV_PATH_SEC) -execcmd "$(RPT_CMD)"; \
	fi
	@echo "--------------------------------------------------"
	@echo "Done. Report generated at: ../coverage_report.log"