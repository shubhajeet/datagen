
/*
 * generate the trace dataset
 * - TODO
 *   1. Define OUTPUT__DT
 *   2. create distribution.yaml file
 *   3. pass the required paramter as argument
 *
 *   Distribution supported:
 *   - linear
 *   -
 */

#include "flags.h"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <math.h>
#include <cmath>
// #include "spdlog/spdlog.h"
#include <stdlib.h>
#include <random>
#include <algorithm>
#include <gflags/gflags.h>

#define OUTPUT_DT uint32_t

DEFINE_string(distribution, "distribution.yaml", "distribution file");
DEFINE_string(output_file, "output.csv", "output file");

int main(int argc, char *argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  auto distribution = FLAGS_distribution;
  auto file_name = FLAGS_output_file;
  // Default values
  // spdlog::set_level(spdlog::level::warn);
  // std::string distribution = "/home/sujit/data/syn_data/linseg50per50ran.yaml";
  // std::string file_name = "output.csv";
  // // Command line parsing
  // if (argc >= 2)
  // {
  //   auto flags = parse_flags(argc, argv);
  //   distribution = get_required(flags, "distribution");
  //   file_name = get_required(flags, "output_file");
  // }
  // spdlog::info("distrbution_file: {} output_file: {}", distribution, file_name);
  std::cout << "distrbution_file: " << distribution << " output_file: " << file_name << std::endl;
  // YAML load
  YAML::Node dist = YAML::LoadFile(distribution);
  // Find the length of the whole dataset
  auto length = 0;
  for (std::size_t i = 0; i < dist.size(); i++)
  {
    auto pice = dist[i];
    length = length + pice["length"].as<OUTPUT_DT>();
  }
  // spdlog::info("Dataset_length: {} ", length);
  std::cout << "Dataset_length: " << length << std::endl;
  // Declare the final dataset
  std::vector<OUTPUT_DT> vec(length);
  auto ptr = vec.begin();
  OUTPUT_DT start = 0;
  // loop through yaml file
  for (std::size_t i = 0; i < dist.size(); i++)
  {
    auto pice = dist[i];
    auto distribution = pice["distribution"].as<std::string>();
    // linear dataset support
    if (distribution == "linear")
    {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      // start = start + offset;
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto step = double(stop - offset) / dis_len;
      // auto randomness = pice["randomness"].as<double>();
      // spdlog::info("distribution: linear start: {} offset: {} stop: {} step: {} length: {}", start, offset, stop, step, dis_len);
      std::cout << "distribution: linear start: " << start << " offset: " << offset << " stop: " << stop << " step: " << step << " length: " << dis_len << std::endl;
      // auto random_data_size = randomness * dis_len;
      auto linstep = [=]()
      {
        static OUTPUT_DT output = start;
        // float ranno = (float) rand() / RAND_MAX;
        // spdlog::debug("random_no: {}",ranno);
        output = output + step;
        // spdlog::debug("lingen output: {}", output);
        std::cout << "lingen output: " << output << std::endl;
        return OUTPUT_DT(output);
      };
      /*
      auto rangen = [=]() {
        float ranno = (double) rand() / RAND_MAX;
        //spdlog::debug("random_no: {}",ranno);
        auto output = (stop - offset) * ranno + start;
        spdlog::debug("rangen output: {}",output);
        return OUTPUT_DT(output);
      };
      */
      // std::generate(ptr, ptr+random_data_size, rangen );
      // std::generate(ptr+random_data_size, ptr+dis_len, linstep);
      std::generate(ptr, ptr + dis_len, linstep);
      // std::sort(ptr,ptr+dis_len);
      start = start + dis_len * step;
      ptr = ptr + dis_len;
    }
    else if (distribution == "random")
    {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      // start = start + offset;
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto step = (stop - offset) / dis_len - 1;
      auto randomness = pice["randomness"].as<float>();
      // auto RAND_FACTOR = step;
      auto RAND_FACTOR = pice["rand_fact"].as<float>();
      if (RAND_FACTOR > step)
      {
        RAND_FACTOR = step;
      }
      // spdlog::info("distribution: random start: {} offset: {} stop: {} step: {} length: {}", start, offset, stop, step, dis_len);
      std::cout << "distribution: random start: " << start << " offset: " << offset << " stop: " << stop << " step: " << step << " length: " << dis_len << std::endl;
      OUTPUT_DT output = 0;
      for (auto ptr_ = ptr; ptr_ <= (ptr + dis_len); ptr_++)
      {
        float ranno = (float)rand() / RAND_MAX;
        auto step = static_cast<OUTPUT_DT>(1 + RAND_FACTOR * randomness * ranno);
        auto prev_output = output;
        output = output + step;
        // spdlog::warn("rangen prev_output: {} output: {} step: {} ", prev_output, output, step);
        std::cout << "rangen prev_output: " << prev_output << " output: " << output << " step: " << step << std::endl;
        *ptr_ = output;
      }
      auto first = *ptr;
      auto last = *(ptr + dis_len - 1);
      auto scalefactor = static_cast<double>(stop - start) / static_cast<double>(last - first);
      if (scalefactor < 1)
      {
        // spdlog::error("Scaled factor is less than 1. scaled_factor: {} first: {} last: {}", scalefactor, first, last);
        std::cerr << "Scaled factor is less than 1. scaled_factor: " << scalefactor << " first: " << first << " last: " << last << std::endl;
      }
      else
      {
        // spdlog::debug("scaled_factor: {} first: {} last: {}", scalefactor, first, last);
        std::cout << "scaled_factor: " << scalefactor << " first: " << first << " last: " << last << std::endl;
      }
      auto scale = [=](const OUTPUT_DT val)
      {
        auto outval = std::round(start + (val - first) * scalefactor);
        // spdlog::debug("rand_factor: {} start: {} first: {} last: {} stop: {} scaled_output: {}", val, start, first, last, stop, outval);
        std::cout << "rand_factor: " << val << " start: " << start << " first: " << first << " last: " << last << " stop: " << stop << " scaled_output: " << outval << std::endl;
        return outval;
      };
      std::transform(ptr, ptr + dis_len, ptr, scale);
      // std::sort(ptr,ptr+dis_len);
      start = start + dis_len * step;
      ptr = ptr + dis_len;
    }
    else if (distribution == "normal")
    {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      start = offset;
      // start += offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto mean = pice["mean"].as<double>();
      auto stddev = pice["stddev"].as<double>();
      auto SCALE = pice["scale"].as<double>();
      auto crop_range = pice["crop_range"].as<double>();
      // spdlog::info("distribution: normal start: {} offset: {} stop: {}  length: {}", start, offset, stop, dis_len);
      std::cout << "distribution: normal start: " << start << " offset: " << offset << " stop: " << stop << " length: " << dis_len << std::endl;
      auto start_norm = mean - crop_range * stddev;
      auto step_norm = 2 * crop_range * stddev / dis_len;
      // auto step = float(stop - start) / dis_len;
      auto normdis = [=](float pos)
      {
        return 1 / (stddev * sqrt(2 * 3.14)) * exp(-(pos - mean) * (pos - mean) / (2 * stddev * stddev));
      };
      /*
      auto normstepgen = [=]() {
        static OUTPUT_DT output = 0;
        auto static pos = start_norm;
        output = std::max(1.0, SCALE * normdis(pos)) + output;
        pos = pos + step_norm;
        spdlog::debug("normal output: {}",output);
        return OUTPUT_DT(output);
      };
      std::generate(ptr, ptr+dis_len, normstepgen );
      */
      OUTPUT_DT output = 0;
      auto pos = start_norm;
      for (auto ptr_ = ptr; ptr_ < (ptr + dis_len); ptr_++)
      {
        output = std::max(1.0, SCALE * normdis(pos)) + output;
        pos = pos + step_norm;
        // spdlog::debug("normal output: {}", output);
        std::cout << "normal output: " << output << std::endl;
        *ptr_ = OUTPUT_DT(output);
      };
      auto first = *ptr;
      auto last = *(ptr + dis_len - 1);
      auto scalefactor = double(stop - start) / double(last - first);
      if (scalefactor < 1)
      {
        // spdlog::error("Scaled factor is less than 1. scaled_factor: {} first: {} last: {}", scalefactor, first, last);
        std::cerr << "Scaled factor is less than 1. scaled_factor: " << scalefactor << " first: " << first << " last: " << last << std::endl;
      }
      else
      {
        // spdlog::debug("scaled_factor: {} first: {} last: {}", scalefactor, first, last);
        std::cout << "scaled_factor: " << scalefactor << " first: " << first << " last: " << last << std::endl;
      }
      auto scale = [=](const OUTPUT_DT val)
      {
        auto outval = start + double(val - first) * scalefactor;
        // spdlog::debug("scaled_output: {}", outval);A
        std::cout << "scaled_output: " << outval << std::endl;
        return outval;
      };

      std::transform(ptr, ptr + dis_len, ptr, scale);
      // std::sort(ptr,ptr+dis_len);
      start = *(ptr + dis_len - 1);
      ptr = ptr + dis_len;
    }
    else if (distribution == "lognormal")
    {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      // start += offset;
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto mean = pice["mean"].as<double>();
      auto stddev = pice["stddev"].as<double>();
      auto SCALE = pice["scale"].as<double>();
      auto crop_start = pice["crop_start"].as<double>();
      auto crop_stop = pice["crop_stop"].as<double>();
      // spdlog::info("distribution: lognormal start: {} offset: {} stop: {}  length: {}", start, offset, stop, dis_len);
      std::cout << "distribution: lognormal start: " << start << " offset: " << offset << " stop: " << stop << " length: " << dis_len << std::endl;
      auto start_norm = crop_start;
      auto step_norm = double(crop_stop - crop_start) / dis_len;
      // auto step = float(stop - start) / dis_len;
      auto normdis = [=](float pos)
      {
        return 1 / (stddev * sqrt(2 * 3.14)) * exp(-(pos - mean) * (pos - mean) / (1 * stddev * stddev));
      };
      /*
      auto normstepgen = [=]() {
        static OUTPUT_DT output = 0;
        auto static pos = start_norm;
        output = std::max(1.0, SCALE * normdis(logf(pos))/pos) + output;
        pos = pos + step_norm;
        //spdlog::debug("lognormal output: {}",output);
        return OUTPUT_DT(output);
      };
      std::generate(ptr, ptr+dis_len, normstepgen );
      */
      OUTPUT_DT output = 0;
      auto pos = start_norm;
      for (auto ptr_ = ptr; ptr_ < (ptr + dis_len); ptr_++)
      {
        output = std::max(1.0, SCALE * normdis(logf(pos)) / pos) + output;
        pos = pos + step_norm;
        // spdlog::debug("lognormal output: {}", output);
        std::cout << "lognormal output: " << output << std::endl;
        *ptr_ = output;
      }
      auto first = *ptr;
      auto last = *(ptr + dis_len - 1);
      auto scalefactor = double(stop - start) / double(last - first);
      if (scalefactor < 1)
      {
        // spdlog::error("Scaled factor is less than 1. scaled_factor: {} first: {} last: {}", scalefactor, first, last);
        std::cerr << "Scaled factor is less than 1. scaled_factor: " << scalefactor << " first: " << first << " last: " << last << std::endl;
      }
      else
      {
        // spdlog::debug("scaled_factor: {} first: {} last: {}", scalefactor, first, last);
        std::cout << "scaled_factor: " << scalefactor << " first: " << first << " last: " << last << std::endl;
      }
      auto scale = [=](const OUTPUT_DT val)
      {
        auto outval = start + double(val - first) * scalefactor;
        // spdlog::debug("scaled_output: {}", outval);
        std::cout << "scaled_output: " << outval << std::endl;
        return outval;
      };
      std::transform(ptr, ptr + dis_len, ptr, scale);
      // std::sort(ptr,ptr+dis_len);
      start = *(ptr + dis_len - 1);
      ptr = ptr + dis_len;
    }
    else if (distribution == "exponential")
    {
      auto dis_len = pice["length"].as<OUTPUT_DT>();
      auto offset = pice["start"].as<OUTPUT_DT>();
      start = offset;
      auto stop = pice["stop"].as<OUTPUT_DT>();
      auto base = pice["base"].as<double>();
      // spdlog::info("distribution: exponential start: {} offset: {} stop: {}  length: {}", start, offset, stop, dis_len);
      std::cout << "distribution: exponential start: " << start << " offset: " << offset << " stop: " << stop << " length: " << dis_len << std::endl;
      auto expgen = [=]()
      {
        static auto power = 0;
        power = power + 1;
        return OUTPUT_DT(start + std::pow(base, power));
      };
      std::generate(ptr, ptr + dis_len, expgen);
      auto first = *ptr;
      auto last = *(ptr + dis_len - 1);
      auto scalefactor = double(stop - start) / double(last - first);
      if (scalefactor < 1)
      {
        // spdlog::error("Scaled factor is less than 1. scaled_factor: {}", scalefactor);
        std::cerr << "Scaled factor is less than 1. scaled_factor: " << scalefactor << std::endl;
      }
      else
      {
        // spdlog::debug("scaled_factor: {}", scalefactor);
        std::cout << "scaled_factor: " << scalefactor << std::endl;
      }
      auto scale = [=](const OUTPUT_DT val)
      {
        auto outval = start + double(val - first) * scalefactor;
        // spdlog::debug(" scaled_output: {}", outval );
        return outval;
      };
      std::transform(ptr, ptr + dis_len, ptr, scale);
      // std::sort(ptr,ptr+dis_len);
      start = *(ptr + dis_len - 1);
      ptr = ptr + dis_len;
    }
  }
  // sort and save the generated dataset
  // display(vec.begin(),vec.end());
  // std::sort(vec.begin(),vec.end());
  record(vec.begin(), vec.end(), file_name);
  return 0;
}
