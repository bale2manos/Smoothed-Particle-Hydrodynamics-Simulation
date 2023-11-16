#include "sim/progargs.hpp"
#include "sim/block.hpp"
#include <vector>
#include <iostream>





int main(int argc, const char* argv[]) {

  std::vector<std::string> argvparams(argv + 1, argv + argc);
  std::array<int,2> params = validate_parameters(argc, argvparams); /* TODO esto se puede mejorar */
  if (params[0] < 0) { return params[0];}
  // try catch of read_input_file and any exception
  try {
    Malla malla = read_input_file(argvparams[1].c_str());
    for (int iter = 0; iter < stoi(argvparams[0]); iter++) {
      malla.mallaInteraction();
    }
    write_output_file(malla, argvparams[2].c_str());

  }
  catch (const runtime_error& e) {
    cerr << e.what() << "\n";
    return -5;
  }

  return 0;
}




