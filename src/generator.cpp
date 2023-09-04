
/**
 * Executable to generate the dataset of differnt distribution
 * TODO
 *  1. Set OUTPUT_DT to datatype needed
 *  2. pass the yaml file containing the information of the dataset you want to generate
 *  3. pass the output file name ( the filename ends with csv for text data, does not end with csv for binary data)
 *
 */
#include <iostream>
#include <string.h>
#include "samplegen.h"
#include "flags.h"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <math.h>
// #include "spdlog/spdlog.h"
#include <stdlib.h>
#include <gflags/gflags.h>

#define OUTPUT_DT uint32_t

DEFINE_string(distribution, "distribution.yaml", "distribution file");
DEFINE_string(output_file, "output.csv", "output file");

int main(int argc, char *argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  auto distribution = FLAGS_distribution;
  auto file_name = FLAGS_output_file;
  // spdlog::set_level(spdlog::level::debug);
  // Default parameter
  // std::string distribution = "distribution_ran.yaml";
  // std::string file_name = "output.csv";

  // // Check command line paramter
  // if (argc >= 2)
  // {
  //   auto flags = parse_flags(argc, argv);
  //   distribution = get_required(flags, "distribution");
  //   file_name = get_required(flags, "output_file");
  // }
  // spdlog::info("distrbution_file: {} output_file: {}", distribution, file_name);

  // Loads the yaml file
  YAML::Node dist = YAML::LoadFile(distribution);

  // First calculate the length of the dataset that needs to be generated
  auto length = 0;
  for (std::size_t i = 0; i < dist.size(); i++)
  {
    auto pice = dist[i];
    length = length + pice["length"].as<OUTPUT_DT>();
  }
  // spdlog::info("Dataset_length: {} ", length);
  std::cout << "Dataset_length: " << length << std::endl;

  // Declare vector of required length
  std::vector<OUTPUT_DT> vec(length);

  auto ptr = vec.begin();
  OUTPUT_DT start = 0;
  /*
   * Expected YAML File syntax
   * - distribution: linear
   *    start: <int>
   *    stop: <int>
   *    length: <int>
   */
  for (std::size_t i = 0; i < dist.size(); i++)
  {
    auto pice = dist[i];
    auto distribution = pice["distribution"].as<std::string>();
    if (distribution == "linear")
    {
      auto dis_len = pice["length"].as<int>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      start = start + offset;
      // start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto step = (stop - offset) / dis_len;

      auto randomness = pice["randomness"].as<float>();
      // spdlog::info("distribution: linear start: {} offset: {} step: {} length: {}", start, offset, step, dis_len);
      std::cout << "distribution: linear start: " << start << " offset: " << offset << " step: " << step << " length: " << dis_len << std::endl;

      auto linstep = [=]()
      {
        static OUTPUT_DT output = start;
        float ranno = (float)rand() / RAND_MAX;
        // spdlog::debug("random_no: {}",ranno);
        output = output + step;
        return OUTPUT_DT(output - step * randomness * ranno);
      };

      std::generate(ptr, ptr + dis_len, linstep);
      start = start + dis_len * step;
      ptr = ptr + dis_len;
    }
  }
  // display(vec.begin(),vec.end());
  record(vec.begin(), vec.end(), file_name);
  return 0;
}
