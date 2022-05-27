#include <iostream>
#include <string.h>
#include "samplegen.h"
#include "flags.h"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <math.h>
#include "spdlog/spdlog.h"
#include <stdlib.h>

#define OUTPUT_DT uint32_t

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::debug);
  std::string distribution = "distribution_ran.yaml";
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
    length = length + pice["length"].as<OUTPUT_DT>();
  }
  spdlog::info("Dataset_length: {} ",length);
  std::vector<OUTPUT_DT> vec(length);
  auto ptr = vec.begin();
  auto start = 0;
  for (std::size_t i = 0; i < dist.size(); i++) {
    auto pice = dist[i];
    auto distribution = pice["distribution"].as<std::string>();
    if (distribution == "linear") {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      //start = start + offset;
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto step = (stop - offset)/ dis_len;
      auto randomness = pice["randomness"].as<float>();
      spdlog::info("distribution: linear start: {} offset: {} step: {} length: {}", start, offset, step, dis_len);
      auto linstep = [=]() {
        static OUTPUT_DT output = start;
        float ranno = (float) rand() / RAND_MAX;
        //spdlog::debug("random_no: {}",ranno);
        output = output + step;
        return OUTPUT_DT(output - step * randomness * ranno);
      };
      std::generate(ptr,ptr+dis_len, linstep );
      start = start + dis_len * step;
      ptr = ptr + dis_len;
    }  
  }
  //display(vec.begin(),vec.end());
  record(vec.begin(),vec.end(),file_name);
  return 0;
}