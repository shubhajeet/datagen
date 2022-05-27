
#include "samplegen.h"
#include "flags.h"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <math.h>
#include <cmath>
#include "spdlog/spdlog.h"
#include <stdlib.h>
#include <random>
#include <algorithm>
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
      spdlog::info("distribution: linear start: {} offset: {} stop: {} step: {} length: {}", start, offset, stop, step, dis_len);
      auto random_data_size = randomness * dis_len;
      auto linstep = [=]() {
        static OUTPUT_DT output = start;
        //float ranno = (float) rand() / RAND_MAX;
        //spdlog::debug("random_no: {}",ranno);
        output = output + step;
        spdlog::debug("lingen output: {}",output);
        return OUTPUT_DT(output);
      };
      auto rangen = [=]() {
        float ranno = (float) rand() / RAND_MAX;
        //spdlog::debug("random_no: {}",ranno);
        auto output = (stop - offset) * ranno + start;
        spdlog::debug("rangen output: {}",output);
        return OUTPUT_DT(output);
      };
      std::generate(ptr, ptr+random_data_size, rangen );
      std::generate(ptr+random_data_size, ptr+dis_len, linstep);
      //std::sort(ptr,ptr+dis_len);
      start = start + dis_len * step;
      ptr = ptr + dis_len;
    }
    else if (distribution == "random") {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      //start = start + offset;
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto step = (stop - offset)/ dis_len;
      auto randomness = pice["randomness"].as<float>();
      auto RAND_FACTOR = pice["rand_fact"].as<float>();
      spdlog::info("distribution: random start: {} offset: {} stop: {} step: {} length: {}", start, offset, stop, step, dis_len);
      auto unqrangen = [=]() {
        static std::vector<OUTPUT_DT> prev_output = {};
        static OUTPUT_DT output = 0;
        do {
          float ranno = (float) rand() / RAND_MAX;
          //spdlog::debug("random_no: {}",ranno);
          output = 1 + RAND_FACTOR * randomness * ranno + output;
          spdlog::debug("rangen output: {}",output);
        }
        while ( std::find(prev_output.begin(), prev_output.end(), output) != prev_output.end() );
        return OUTPUT_DT(output);
      };
      std::generate(ptr, ptr+dis_len, unqrangen );
      auto first = *ptr;
      auto last = *(ptr+dis_len-1);
      auto scale = [=](const OUTPUT_DT val) {
        return start + (val-first) * (stop - start)/(last - first);
      };
      std::transform(ptr, ptr+dis_len, ptr, scale );
      //std::sort(ptr,ptr+dis_len);
      start = start + dis_len * step;
      ptr = ptr + dis_len;
    }
    else if (distribution == "normal") {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto mean = pice["mean"].as<double>();
      auto stddev = pice["stddev"].as<double>();
      auto SCALE = pice["scale"].as<double>();
      auto crop_range = pice["crop_range"].as<double>();
      spdlog::info("distribution: normal random start: {} offset: {} stop: {}  length: {}", start, offset, stop, dis_len);
      auto start_norm = mean - crop_range * stddev;
      auto step_norm = 2 * crop_range * stddev / dis_len;
      //auto step = float(stop - start) / dis_len;
      auto normdis = [=](float pos) {
        return  1/(stddev*sqrt(2*3.14)) * exp(-(pos-mean)*(pos-mean)/(2*stddev*stddev));
      };
      auto normstepgen = [=]() {
        static OUTPUT_DT output = 0;
        auto static pos = start_norm;
        output = std::max(1.0, SCALE * normdis(pos)) + output;
        pos = pos + step_norm;
        spdlog::debug("normal output: {}",output);
        return OUTPUT_DT(output);
      };
      std::generate(ptr, ptr+dis_len, normstepgen );
      auto first = *ptr;
      auto last = *(ptr+dis_len-1);
      auto scale = [=](const OUTPUT_DT val) {
        return start + (val-first) * (stop - start)/(last - first);
      };
      std::transform(ptr, ptr+dis_len, ptr, scale );
      //std::sort(ptr,ptr+dis_len);
      start = *(ptr + dis_len -1);
      ptr = ptr + dis_len;
    }  
    else if (distribution == "lognormal") {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto mean = pice["mean"].as<double>();
      auto stddev = pice["stddev"].as<double>();
      auto SCALE = pice["scale"].as<double>();
      auto crop_start = pice["crop_start"].as<double>();
      auto crop_stop = pice["crop_stop"].as<double>();
      spdlog::info("distribution: normal random start: {} offset: {} stop: {}  length: {}", start, offset, stop, dis_len);
      auto start_norm = crop_start;
      auto step_norm = (crop_stop - crop_start)/ dis_len;
      //auto step = float(stop - start) / dis_len;
      auto normdis = [=](float pos) {
        return  1/(stddev*sqrt(2*3.14)) * exp(-(pos-mean)*(pos-mean)/(2*stddev*stddev));
      };
      auto normstepgen = [=]() {
        static OUTPUT_DT output = 0;
        auto static pos = start_norm;
        output = std::max(1.0, SCALE * normdis(logf(pos))/pos) + output;
        pos = pos + step_norm;
        //spdlog::debug("normal output: {}",output);
        return OUTPUT_DT(output);
      };
      std::generate(ptr, ptr+dis_len, normstepgen );
      auto first = *ptr;
      auto last = *(ptr+dis_len-1);
      auto scale = [=](const OUTPUT_DT val) {
        return start + (val-first) * (stop - start)/(last - first);
      };
      std::transform(ptr, ptr+dis_len, ptr, scale );
      //std::sort(ptr,ptr+dis_len);
      start = *(ptr + dis_len -1);
      ptr = ptr + dis_len;
    }
    else if (distribution == "exponential") {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto base = pice["base"].as<double>();
      spdlog::info("distribution: exponential start: {} offset: {} stop: {}  length: {}", start, offset, stop, dis_len);
      auto expgen = [=]() {
        static auto power = 0;
        power = power + 1;
        return OUTPUT_DT(start + std::pow(base,power));
      };
      std::generate(ptr, ptr+dis_len, expgen );
      auto first = *ptr;
      auto last = *(ptr+dis_len-1);
      auto scale = [=](const OUTPUT_DT val) {
        return start + (val-first) * (stop - start)/(last - first);
      };
      //std::transform(ptr, ptr+dis_len, ptr, scale );
      //std::sort(ptr,ptr+dis_len);
      start = *(ptr + dis_len -1);
      ptr = ptr + dis_len;
    }
  }
  //display(vec.begin(),vec.end());
  std::sort(vec.begin(),vec.end());
  record(vec.begin(),vec.end(),file_name);
  return 0;
}
