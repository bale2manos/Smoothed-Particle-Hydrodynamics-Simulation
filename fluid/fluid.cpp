#include "sim/block.hpp"
#include "sim/progargs.hpp"

#include <iostream>
#include <vector>

int main(int argc, char const * argv[]) {
  std::vector<std::string> argvparams(argv + 1, argv + argc);
  std::array<int, 2> params =
      validate_parameters(argc, argvparams); /* TODO esto se puede mejorar */
  if (params[0] < 0) { return params[0]; }
  Malla malla = {};

  // try catch of read_input_file and any exception
  try {
    malla = read_input_file(argvparams[1].c_str());
  } catch (std::runtime_error const & e) {
    std::cerr << e.what() << "\n";
    return wrong_particle_number;
  }

  malla.mallaInteraction(stoi(argvparams[0])
  write_output_file(malla, argvparams[2].c_str());

  return 0;
}
