#include "sim/progargs.hpp"
#include "sim/block.hpp"
#include <vector>
#include <iostream>





int main(int argc, const char *argv[]) {

  std::vector<string> argvparams;
  int const argcparam = argc;
  for(int i = 0; i < argcparam; i++){
    argvparams.emplace_back(argv[i]);
  }
  std::array<int,2> params = validate_parameters(argcparam, argvparams); /* TODO esto se puede mejorar */
  if (params[0] < 0) { return params[0];}

  // try catch of read_input_file and any exception
  int const num_iterations = stoi(argvparams[1]);
  try {
    Malla malla = read_input_file(argvparams[2].c_str());
    for (int iter = 0; iter < num_iterations; iter++) {
      malla.mallaInteraction();
    }
    write_output_file(malla, argvparams[3].c_str());

  } catch (const runtime_error& e) {
    cerr << e.what() << "\n";
    return -5;
  }

  return 0;
}




