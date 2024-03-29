
#include "flags.h"
#include "utils.h"
#include "trace_helper.h"
#include <math.h>
#include <cmath>
// #include "spdlog/spdlog.h"
#include <stdlib.h>
#include <random>
#include <algorithm>
#include <gflags/gflags.h>

#define OUTPUT_DT uint32_t

DEFINE_string(first_distribution, "/home/sujit/experiment/linseg/gendata/lognormal_large_sorted.csv", "first distribution file");
DEFINE_string(second_distribution, "2klinear.csv", "second distribution file");
DEFINE_string(output_file, "lognormal_10000000.csv", "output file");
DEFINE_int32(offset, 10000000, "offset");

int main(int argc, char *argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  auto first_distribution = FLAGS_first_distribution;
  auto second_distribution = FLAGS_second_distribution;
  auto output_file = FLAGS_output_file;
  auto offset = FLAGS_offset;
  // std::string first_distribution = "/home/sujit/experiment/linseg/gendata/lognormal_large_sorted.csv";
  // std::string second_distribution = "2klinear.csv";
  // std::string output_file = "lognormal_10000000.csv";
  // int offset = 10000000;
  // // Check command line paramter
  // if (argc >= 4)
  // {
  //   auto flags = parse_flags(argc, argv);
  //   first_distribution = get_required(flags, "first");
  //   second_distribution = get_required(flags, "second");
  //   offset = std::stoi(get_required(flags, "offset"));
  //   output_file = get_required(flags, "output_file");
  // }

  // spdlog::info("first_distrbution_file: {} second_distribution_file: {} output_file: {}", first_distribution, second_distribution, output_file);
  std::cout << "first_distrbution_file: " << first_distribution << " second_distribution_file: " << second_distribution << " output_file: " << output_file << std::endl;
  std::vector<OUTPUT_DT> data;
  if (first_distribution.compare(first_distribution.size() - 4, 4, ".csv") == 0)
  {
    // spdlog::info("csv file: {}", first_distribution);
    std::cout << "csv file: " << first_distribution << std::endl;
    load_text_data(data, first_distribution);
  }
  else
  {
    // spdlog::info("trace file: {}", first_distribution);
    std::cout << "trace file: " << first_distribution << std::endl;
    load_vector(data, first_distribution);
  }
  std::vector<OUTPUT_DT> seconddata;
  if (second_distribution.compare(second_distribution.size() - 4, 4, ".csv") == 0)
  {
    // spdlog::info("csv file: {}", second_distribution);
    std::cout << "csv file: " << second_distribution << std::endl;
    load_text_data(seconddata, second_distribution);
  }
  else
  {
    // spdlog::info("trace file: {}", second_distribution);
    std::cout << "trace file: " << second_distribution << std::endl;
    load_vector(seconddata, second_distribution);
  }
  if (offset > data.size())
  {
    // spdlog::error("Offset size greater than first data size offset: {} data_size: {}", offset, data.size());
    std::cerr << "Offset size greater than first data size offset: " << offset << " data_size: " << data.size() << std::endl;
  }
  std::vector<OUTPUT_DT> vec;
  for (auto itr = data.begin(); itr < (data.begin() + offset); itr++)
  {
    // std::cout << *itr << std::endl;
    vec.push_back(*itr);
  }
  auto last_value = data[offset - 1];
  for (auto itr = seconddata.begin(); itr < seconddata.end(); itr++)
  {
    // std::cout << *itr + last_value << std::endl;
    vec.push_back(*itr + last_value);
  }
  last_value = data[data.size() - 1];
  for (auto itr = (data.begin() + offset); itr < data.end(); itr++)
  {
    // std::cout << *itr+last_value << std::endl;
    vec.push_back(*itr + last_value);
  }
  // std::sort(data.begin(),data.end());
  if (output_file.compare(output_file.size() - 4, 4, ".csv") == 0)
  {
    // spdlog::info("csv file: {}", output_file);
    std::cout << "csv file: " << output_file << std::endl;
    to_csv(vec, output_file);
  }
  else
  {
    // spdlog::info("trace file: {}", output_file);
    std::cout << "trace file: " << output_file << std::endl;
    to_binary_file(vec, output_file);
  }
  return 0;
}
