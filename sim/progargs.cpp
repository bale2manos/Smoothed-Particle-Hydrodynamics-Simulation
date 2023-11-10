#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "progargs.hpp"
#include "block.hpp"
#include "grid.hpp"

#include <filesystem>

using namespace std;
int validate_time_steps(int nts) {
  if (nts == 0) {
    cerr << "Error: time steps must be numeric." << "\n";
    return -1;
  }
  if (nts < 0) {
    cerr << "Error: Invalid number of time steps." << "\n";
    return -2;
  }
  return 0;
}

int validate_input_file(const char* inputFileName) {
  cout << "Input file: " << inputFileName << "\n";
  ifstream input_file(inputFileName);
  if (!input_file.is_open()) {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentPath << std::endl;
    cerr << "Error: Cannot open " << inputFileName << " for reading." << "\n";
    return -1;
  }


  return 0;
}

int validate_output_file(const char* outputFileName) {
  ofstream output_file(outputFileName);
  if (!output_file.is_open()) {
    cerr << "Error: Cannot open " << outputFileName << " for writing." << "\n";
    return -1;
  }
  return 0;
}

array<int,2> validate_parameters(int argc, const char* argv[]) {

  array <int,2> error_type={0,0};
  if (argc != 4) {
    cerr << "Error: Invalid number of arguments: " << argc << "\n";
    error_type[0] = -1;
    return error_type;
  }

  error_type[1] = stoi(argv[1]);
  error_type[0] = validate_time_steps(error_type[1]);
  if (error_type[0] != 0) {
    return error_type;
  };
  if (validate_input_file(argv[2]) != 0)
  {
    error_type[0] = -3;
    return error_type;
  }
  if (validate_output_file(argv[3]) != 0){
    error_type[0] = -4;
    return error_type;
  }
  return error_type;
}

Malla read_input_file (const char * in_file) {

  ifstream input_file(in_file, ios::binary);     /* TODO ppm check errors? */

  // Crear la malla base
  float ppm=0;
  input_file.read(reinterpret_cast<char *>(&ppm), sizeof(ppm));
  int np=0;
  input_file.read(reinterpret_cast<char *>(&np), sizeof(np));
  //Comprobamos que np sea mayor que 1
  check_np(np);

  // Calculate constants TODO crear archivo constantes
  // Check if the number of particles read matches the header
  auto ppm_double = static_cast<double>(ppm);

  /* TODO pasar struct constantes a malla*/
  // Creamos la malla y la llenamos de bloques vacíos
  //create_fill_grid(malla, np, ppm_double);
  Malla grid(np, ppm_double);
  grid.insert_particles(in_file);

  // Read particle data in a single loop and cast from float to double

  input_file.close(); /* TODO esto hay que cerrarlo? */

  cout << "Number of particles: " << np << "\n";
  cout << "Particles per meter: " << ppm << "\n";
  cout << "Smoothing length: " << grid.getH() << "\n";
  cout << "Particle mass: " << grid.particle_mass() << "\n";
  cout << "Grid size: " << grid.getNBlocks()[0] << " x " << grid.getNBlocks()[1]  << " x " << grid.getNBlocks()[2] << "\n";
  cout << "Number of blocks: " <<  grid.getNBlocks()[0] *  grid.getNBlocks()[1] *  grid.getNBlocks()[2] << "\n";
  cout << "Block size: " << grid.getSizeBlocks()[0] << " x " << grid.getSizeBlocks()[1] << " x " << grid.getSizeBlocks()[2] << "\n";
  return grid;
}



int write_output_file (Malla& malla, const char * out_file){
  int np = malla.getNp();
  double ppm = malla.getPpm();
  //Escribir en el archivo np y ppm antes de entrar en el bucle para las partículas
  ofstream output_file(out_file, ios::binary);
  output_file.write(reinterpret_cast<char*>(&ppm), sizeof(ppm));
  output_file.write(reinterpret_cast<char*>(&np), sizeof(np));

  float px_float, py_float, pz_float, hvx_float, hvy_float, hvz_float, vx_float, vy_float, vz_float;
  vector<Particle> particles_out;
  // Loop through all the blocks
  for (Particle & particle: malla.getParticles()){
      particles_out.push_back(particle);
  }


  // Sort particles_out by id
  sort(particles_out.begin(), particles_out.end(), [](Particle & a, Particle & b) { return a.id < b.id; });

  for (Particle & particle : particles_out) {
    px_float = static_cast<float>(particle.p[0]);
    py_float = static_cast<float>(particle.p[1]);
    pz_float = static_cast<float>(particle.p[2]);
    hvx_float = static_cast<float>(particle.hv[0]);
    hvy_float = static_cast<float>(particle.hv[1]);
    hvz_float = static_cast<float>(particle.hv[2]);
    vx_float = static_cast<float>(particle.v[0]);
    vy_float = static_cast<float>(particle.v[1]);
    vz_float = static_cast<float>(particle.v[2]);
    output_file.write(reinterpret_cast<char*>(&px_float), sizeof(px_float));
    output_file.write(reinterpret_cast<char*>(&py_float), sizeof(py_float));
    output_file.write(reinterpret_cast<char*>(&pz_float), sizeof(pz_float));
    output_file.write(reinterpret_cast<char*>(&hvx_float), sizeof(hvx_float));
    output_file.write(reinterpret_cast<char*>(&hvy_float), sizeof(hvy_float));
    output_file.write(reinterpret_cast<char*>(&hvz_float), sizeof(hvz_float));
    output_file.write(reinterpret_cast<char*>(&vx_float), sizeof(vx_float));
    output_file.write(reinterpret_cast<char*>(&vy_float), sizeof(vy_float));
    output_file.write(reinterpret_cast<char*>(&vz_float), sizeof(vz_float));

    if (particle.id == 0){
    cout << "Particle " << particle.id << " Data:" << "\n";
    cout << "px: " << particle.p[0] << "\n";
    cout << "py: " << particle.p[1] << "\n";
    cout << "pz: " << particle.p[2] << "\n";
    cout << "hvx: " << particle.hv[0] << "\n";
    cout << "hvy: " << particle.hv[1] << "\n";
    cout << "hvz: " << particle.hv[2] << "\n";
    cout << "vx: " << particle.v[0] << "\n";
    cout << "vy: " << particle.v[1] << "\n";
    cout << "vz: " <<particle.v[2] << "\n";

    }


  }

  output_file.close();

  return 0;
}



void check_np (int np){
  if (np <= 0) {
    string errorMsg = "Error: Invalid number of particles: " + to_string(np) + ".\n";
    throw runtime_error(errorMsg);
  }
}








void check_missmatch_particles(int counter, int malla_np) {
  if (counter != malla_np) {
    string errorMsg =
        "Error: Number of particles mismatch. Header: " + to_string(malla_np) + ", Found: " + to_string(counter) +
        ".\n";
    throw runtime_error(errorMsg);
  }
}


