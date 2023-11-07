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
  vector<string> const args(argv, argv + argc);
  array<int,2> params = validate_parameters(argc, argv);        /* TODO esto se puede mejorar */
  if (params[0] < 0) { return params[0];}
  Malla malla;

  // try catch of read_input_file and any exception
  try {
    read_input_file(malla, args[2].c_str());
  } catch (const runtime_error& e) {
    cerr << e.what() << "\n";
    return -5;
  }

  int const num_iterations = atoi(argv[1]);



  for (int iter = 0; iter < num_iterations; iter++) {

    cout << "**************************************" << "\n";
    cout << "ITERACION NUMERO: " << iter << "\n";
    malla_interaction(malla);
  }



  write_output_file(malla, argv[3]);
  // time end
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
  // cast from micoseconds to minutes and seconds
        int minutes = duration.count() / 60000000;
        int seconds = (duration.count() - minutes * 60000000) / 1000000;
        cout << "Time taken by function: " << minutes << " minutes and " << seconds << " seconds" << endl;




  return 0;
}