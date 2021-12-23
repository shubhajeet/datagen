#include <iostream>
#include "samplegen.h"
#include "flags.h"
#include "utils.h"

#define OUTPUT_DT int

int main(int argc, char* argv[]) {
  auto flags = parse_flags(argc, argv);
  std::string distribution = get_required(flags, "distribution");
  std::string file_name = get_required(flags, "output_file");
  auto start = stoi(get_required(flags,"start"));
  auto stop = stoi(get_required(flags,"stop"));
  auto length = stoi(get_required(flags,"length"));
  
  std::vector<OUTPUT_DT> vec(length);
  if (distribution == "linear"){
    std::generate(vec.begin(),vec.end(),linear_gen<OUTPUT_DT>(start,stop));
  }
  else if (distribution == "exponential") {
    std::generate(vec.begin(),vec.end(),exp_gen<OUTPUT_DT>(start,stop));
  }
  else if (distribution == "logarithmic") {
    std::generate(vec.begin(),vec.end(),nlog_gen<OUTPUT_DT>(start,stop));
  }
  else {
    std::cout << "Distribution not found" << std::endl;
    return -1;
  }
  display(vec.begin(),vec.end());
  record(vec.begin(),vec.end(),file_name);
  return 0;
}