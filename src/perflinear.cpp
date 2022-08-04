
/*
 * generate linear dataset
 * - TODO
 * 1. Define OUTPUT_DT
 * 2. create distribution.yaml file
 * 3. pass the required parameter as argument
 * 
 * Distribution supported:
 * - linear
 * 
 * YAML 
 * */
#include <iostream>
#include <string.h>
#include "samplegen.h"
#include "flags.h"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <math.h>
#include "spdlog/spdlog.h"

#define OUTPUT_DT uint32_t

int main(int argc, char* argv[]) {

  spdlog::set_level(spdlog::level::debug);
  std::string distribution = "distribution.yaml";
  std::string file_name = "output.csv";
  if (argc >= 2) {
    auto flags = parse_flags(argc, argv);
    distribution = get_required(flags, "distribution");
    file_name = get_required(flags, "output_file");
  }
  spdlog::info("distrbution_file: {} output_file: {}", distribution, file_name);
  YAML::Node dist = YAML::LoadFile(distribution);
  auto length = 0;
  for (std::size_t i=0; i < dist.size(); i++) {
    auto pice = dist[i];
    length = length + pice["length"].as<int>();
  }
  spdlog::info("Dataset_length: {} ",length);
  std::vector<OUTPUT_DT> vec(length);
  auto ptr = vec.begin();
  auto start = 0;
  for (std::size_t i = 0; i < dist.size(); i++) {
    auto pice = dist[i];
    auto distribution = pice["distribution"].as<std::string>();
    if (distribution == "linear") {
      auto dis_len = pice["length"].as<int>();
      auto offset = pice["offset"].as<int>();
      start = start + offset;
      auto step = pice["step"].as<int>();
      spdlog::info("distribution: linear start: {} offset: {} step: {} length: {}", start, offset, step, dis_len);
      auto linstep = [=]() {
        static int output = start;
        output = output + step;
        return OUTPUT_DT(output); 
      };
      std::generate(ptr,ptr+dis_len, linstep );
      start = start + dis_len * step;
      ptr = ptr + dis_len;
    }  
  }
  display(vec.begin(),vec.end());
  record(vec.begin(),vec.end(),file_name);
  return 0;
}
