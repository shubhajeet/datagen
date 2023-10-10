/***
 * @file unique.cpp
 * @brief This file is used to generate unique dataset from a trace file
 * input : trace file
 * output : unique dataset
 */
#include <iostream>
#include "flags.h"
#include "utils.h"
#include "trace_helper.h"
#include <gflags/gflags.h>

#define OUTPUT_DT uint32_t

DEFINE_string(trace_file, "/mnt/data/trace/trace-u32-le/w106", "trace file");
DEFINE_string(output_file, "/tmp/w106", "output file");

int main(int argc, char *argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);

  auto tracefile = FLAGS_trace_file;
  auto outputfile = FLAGS_output_file;
  std::cout << "trace file: " << tracefile << std::endl;
  std::cout << "output file: " << outputfile << std::endl;
  std::vector<OUTPUT_DT> trace;
  load_vector(trace, tracefile);
  std::sort(trace.begin(), trace.end());
  auto last = std::unique(trace.begin(), trace.end());
  trace.erase(last, trace.end());
  to_binary_file(trace, outputfile);
  return 0;
}