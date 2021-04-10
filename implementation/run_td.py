#!/usr/bin/env python3
# A Python wrapper script to build and run termination detection
# This script should be run from the Implementation

import os
import sys

STTD_BIN = "./bin/sttd"

def run_termination_detection(data_file, proc_count):
    # read the number of nodes for the input data file
    with open(data_file) as fp:
        line_count = 0
        no_of_nodes = 0
        for line in fp.readlines():
            if (line[0] == '#'): continue
            line_count = line_count + 1
            if (line_count == 1):
                no_of_nodes = line.rstrip()
                break
    if (proc_count == ""):
        proc_count = no_of_nodes

    print("[INFO] No of Nodes in the given graph:", no_of_nodes)
    if (int(proc_count) > 1):
        print("[INFO] Initiating Termination Detection with "+proc_count+" processes.")
    else:
        print("[INFO] Initiating Termination Detection with "+proc_count+" process.")

    run_td_cmd = "mpirun -np " + proc_count + " " + STTD_BIN + " " + data_file
    os.system(run_td_cmd)

def main(data_file, proc_count):
    # build the binaries
    print("[INFO] Building binaries ...")
    run_build_cmd = "make clean; make"
    os.system(run_build_cmd)
    # Intiate termination detection
    run_termination_detection(data_file, proc_count)

def print_usage_and_exit():
    print("[USAGE] ./run_td.py <input_file> <no_of_processes>")
    sys.exit()

# script starts here - main
if __name__ == '__main__':
    proc_count = ""
    test_data_file = ""
    if len(sys.argv) == 3:
        test_data_file = sys.argv[1]
        proc_count = sys.argv[2]
    else:
        print("[ERROR] Invalid input arguments.")
        print_usage_and_exit()
    main(test_data_file, proc_count)
