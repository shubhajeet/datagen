
/*
 * generate linear dataset
 * add last mile error to 90% of data except the first and the last one
 * add generate spline error to 10% of data except the first and the last one
 * */
#include <iostream>
#include <string.h>
#include "samplegen.h"
#include "flags.h"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <math.h>
#include <gflags/gflags.h>
#include "trace_helper.h"

#define OUTPUT_DT uint64_t

DEFINE_uint32(length, 200000000, "length of dataset");
DEFINE_uint32(step, 20, "step of dataset");
DEFINE_uint32(spline_error, 10, "spline generation error");
DEFINE_uint32(offset, 0, "offset");
DEFINE_uint32(last_mile_error, 1, "last mile error");
DEFINE_string(filename, "dataset", "filename");

int main(int argc, char *argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  auto length = FLAGS_length;
  auto step = FLAGS_step;
  auto last_mile_error = FLAGS_last_mile_error;
  auto spline_error = FLAGS_spline_error;
  auto offset = FLAGS_offset;
  auto filename = FLAGS_filename;
  std::vector<OUTPUT_DT> vec(length);
  vec[0] = offset;
  for (std::size_t i = 1; i < length; i++)
  {
    vec[i] = offset + i * step;
  }
  // display(vec.begin(),vec.end());
  // introduce last mile error for 90% of data
  for (std::size_t i = 1; i < length - 1; i++)
  {
    auto r = rand() % 10;
    if (r < 9)
    {
      if (rand() % 2 == 0)
      {
        vec[i] += last_mile_error;
      }
      else
      {
        vec[i] -= last_mile_error;
      }
    }
    else
    {
      if (rand() % 2 == 0)
      {
        vec[i] += spline_error;
      }
      else
      {
        vec[i] -= spline_error;
      }
    }
  }
  // record(vec.begin(), vec.end(), filename);
  to_binary_file(vec, filename);
  return 0;
}
