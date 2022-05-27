#include <iostream>
#include <string.h>
#include "samplegen.h"
#include "flags.h"
#include "utils.h"
#include <math.h>

#define OUTPUT_DT uint32_t

int main(int argc, char* argv[]) {
  auto start = 0.0;
  auto stop = 1.0;
  auto length = 1000;
  std::string distribution = "mixcurve";
  std::string file_name = "test.csv";
  if (argc >= 2) {
    auto flags = parse_flags(argc, argv);
    distribution = get_required(flags, "distribution");
    file_name = get_required(flags, "output_file");
    start = std::stof(get_required(flags,"start"));
    stop = std::stof(get_required(flags,"stop"));
    length = stoi(get_required(flags,"length"));
  }
  std::vector<OUTPUT_DT> vec(length);
  if (distribution == "linear"){
    std::generate(vec.begin(),vec.end(),linstep_gen<OUTPUT_DT>(OUTPUT_DT(start),OUTPUT_DT(stop)));
  }
  else if (distribution == "exponential") {
    //std::generate(vec.begin(),vec.end(),exp_gen<OUTPUT_DT>(start,stop));
    std::generate(vec.begin(),vec.end(),expstep_gen<OUTPUT_DT>(start,stop));
  }
  else if (distribution == "logarithmic") {
    //std::generate(vec.begin(),vec.end(),nlog_gen<OUTPUT_DT>(start,stop));
    std::generate(vec.begin(),vec.end(),logstep_gen<OUTPUT_DT>(start,stop,2));
  }
  else if (distribution == "normal") {
    //std::generate(vec.begin(),vec.end(),nlog_gen<OUTPUT_DT>(start,stop));
    //std::generate(vec.begin(),vec.end(),normaldis_gen<OUTPUT_DT>(start,stop));
    std::default_random_engine generator;
    std::normal_distribution distribution(start,stop);
    for(auto i=0; i < length; i++) {
      vec[i] = distribution(generator);
    }
  }
  else if (distribution == "lognormal") {
    //std::generate(vec.begin(),vec.end(),nlog_gen<OUTPUT_DT>(start,stop));
    std::default_random_engine generator;
    std::lognormal_distribution distribution(start,stop);
    for (auto i=0; i < length; i++) {
      auto data = distribution(generator);
      //std::cout << data * 100000 << std::endl;
      vec[i] = data * 100000;
    }
  }
  else if (distribution == "mixlinear") {
    std::default_random_engine generator;
    std::uniform_int_distribution slopedistribution(1,100);
    std::uniform_int_distribution stepdistribution(1,10000);
    auto counter = 0;
    while (counter < length) {
      auto slope = slopedistribution(generator);
      auto size = stepdistribution(generator);
      for(auto i=0; i < size; i++ ) {
        start += slope;
        vec[counter] = start;
        counter++;
        if (counter == length) {
          break;
        }
      }
    }
  }
  else if (distribution == "mixcurve") {
    std::default_random_engine generator;
    std::uniform_int_distribution sizedistribution(1,1000);
    std::uniform_int_distribution powdistribution(1,3);
    auto counter = 0;
    while (counter < length) {
      auto p = powdistribution(generator);
      auto size = sizedistribution(generator);
      for(auto i=0; i < size; i++ ) {
        auto val = start + pow(i, p);
        vec[counter] = val; 
        counter++;
        if (counter == length) {
          break;
        }
      }
      start = start + pow(size,p);
    }
  }
  else {
    std::cout << "Distribution not found" << std::endl;
    return -1;
  }
  //display(vec.begin(),vec.end());
  record(vec.begin(),vec.end(),file_name);
  return 0;
}