#include "sim/progargs.hpp"
#include "sim/block.hpp"
#include <vector>
#include <iostream>

using namespace std;                                /* TODO mala practica?? */

#include <chrono>
using namespace std::chrono;





int main(int argc, const char *argv[]) {

  // Time start
  auto start = high_resolution_clock::now();
  vector<string> argvparams;
  int argcparam = argc;
  for(int i = 0; i < argcparam; i++){
    argvparams.push_back(argv[i]);
  }
  array<int,2> params = validate_parameters(argcparam, argvparams); /* TODO esto se puede mejorar */
  if (params[0] < 0) { return params[0];}

  // try catch of read_input_file and any exception
  int const num_iterations = stoi(argvparams[1]);
  try {
    Malla malla = read_input_file(argvparams[2].c_str());
    for (int iter = 0; iter < num_iterations; iter++) {
      cout << "**************************************" << "\n";
      cout << "ITERACION NUMERO: " << iter << "\n";
      malla.mallaInteraction();
    }
    write_output_file(malla, argvparams[3].c_str());

  } catch (const runtime_error& e) {
    cerr << e.what() << "\n";
    return -5;
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
  // cast from micoseconds to minutes and seconds
  int minutes = duration.count() / 60000000;
  int seconds = (duration.count() - minutes * 60000000) / 1000000;
  cout << "Time taken by function: " << minutes << " minutes and " << seconds << " seconds" << endl;

  return 0;
}




