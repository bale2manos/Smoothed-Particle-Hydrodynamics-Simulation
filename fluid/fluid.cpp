#include "sim/progargs.hpp"
#include "sim/block.hpp"
#include <vector>
#include <iostream>

using namespace std;                                /* TODO mala practica?? */



int main(int argc, const char *argv[]) {
  vector<string> const args(argv, argv + argc);
  array<int,2> params = validate_parameters(argc, argv);        /* TODO esto se puede mejorar */
  if (params[0] < 0) { return params[0];}
  Malla malla;

  // try catch of read_input_file and any exception
  try {
    malla = read_input_file(args[2].c_str());
  } catch (const runtime_error& e) {
    cerr << e.what() << "\n";
    return -5;
  }

  //for (int iter=0; iter<params[1]; iter++){
  malla = malla_interaction(malla);
  //}




  write_output_file(malla, argv[3]);




  return 0;
}