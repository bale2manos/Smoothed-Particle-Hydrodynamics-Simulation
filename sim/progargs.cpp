#include <iostream>
#include <fstream>
#include <string>
#include "progargs.hpp"
#include "block.hpp"
#include "grid.hpp"


using namespace std;

/**
 * Validates the number of time steps.
 * @param nts The number of time steps to validate.
 * @return 0 if the number of time steps is valid, -1 if it's not numeric, -2 if it's negative.
 */
int validate_time_steps(int nts) {
  if (nts < 0) {
    cerr << "Error: Invalid number of time steps." << "\n";
    return -2;
  }
  return 0;
}


/**
 * Validates if the input file can be opened for reading.
 * 
 * @param inputFileName The name of the input file to be validated.
 * @return Returns 0 if the input file can be opened for reading, -1 otherwise.
 */
int validate_input_file(const char* inputFileName) {
  cout << "Input file: " << inputFileName << "\n";
  ifstream input_file(inputFileName);
  if (!input_file.is_open()) {
    cerr << "Error: Cannot open " << inputFileName << " for reading." << "\n";
    return -1;
  }


  return 0;
}


/**
 * Validates if the output file can be opened for writing.
 * @param outputFileName The name of the output file.
 * @return 0 if the file can be opened for writing, -1 otherwise.
 */
int validate_output_file(const char* outputFileName) {
  ofstream output_file(outputFileName);
  if (!output_file.is_open()) {
    cerr << "Error: Cannot open " << outputFileName << " for writing." << "\n";
    return -1;
  }
  return 0;
}

/**
 * Validates the parameters passed to the program.
 * @param argc The number of arguments passed to the program.
 * @param argv An array of C-style strings containing the arguments passed to the program.
 * @return An array of integers representing the error type. If the first element is 0, there is no error. If it is negative, there is an error. The second element contains the number of time steps.
 */


std::array<int, 2> validate_parameters(int argc, vector<string> argv){
  array <int,2> error_type={0,0};
  if (argc != 4) {
    cerr << "Error: Invalid number of arguments: " << argc << "\n";
    error_type[0] = -1;
    return error_type;
  }

  try {
    error_type[1] = stoi(argv[0]);
  } catch (const std::invalid_argument& e) {
    cerr << "Error time steps must be numeric." << "\n";
    error_type[0] = -1; // Or any other suitable error code
    return error_type;
  }

  error_type[0] = validate_time_steps(error_type[1]);
  if (error_type[0] != 0) {return error_type;}
  if (validate_input_file(argv[1].c_str()) != 0)
  {
    error_type[0] = -3;
    return error_type;
  }
  if (validate_output_file(argv[2].c_str()) != 0){
    error_type[0] = -4;
    return error_type;
  }
  return error_type;
}

/**
 * Reads an input file and creates a Malla object with the particle data.
 * 
 * @param in_file The path to the input file.
 * @return A Malla object with the particle data.
 */
Malla read_input_file (const char * in_file) {

  ifstream input_file(in_file, ios::binary);     

  // Crear la malla base
  float ppm=0;
  input_file.read(reinterpret_cast<char *>(&ppm), sizeof(ppm));
  int number_particles=0;
  input_file.read(reinterpret_cast<char *>(&number_particles), sizeof(number_particles));
  //Comprobamos que np sea mayor que 1
  check_np(number_particles);

  // Check if the number of particles read matches the header
  auto ppm_double = static_cast<double>(ppm);


  // Creamos la malla y la llenamos de bloques vacíos
  Malla grid(number_particles, ppm_double);
  grid.insert_particles(in_file);

  input_file.close(); 

  cout << "Number of particles: " << number_particles << "\n";
  cout << "Particles per meter: " << ppm << "\n";
  cout << "Smoothing length: " << grid.getH() << "\n";
  cout << "Particle mass: " << grid.particle_mass() << "\n";
  cout << "Grid size: " << grid.getNBlocks()[0] << " x " << grid.getNBlocks()[1]  << " x " << grid.getNBlocks()[2] << "\n";
  cout << "Number of blocks: " <<  grid.getNBlocks()[0] *  grid.getNBlocks()[1] *  grid.getNBlocks()[2] << "\n";
  cout << "Block size: " << grid.getSizeBlocks()[0] << " x " << grid.getSizeBlocks()[1] << " x " << grid.getSizeBlocks()[2] << "\n";
  return grid;
}



/**
 * Writes the particle data to a binary file.
 * 
 * @param malla The mesh containing the particles.
 * @param out_file The output file path.
 * @return 0 if the file was successfully written, -1 otherwise.
 */
int write_output_file (Malla& malla, const char * out_file){
  int number_particles = malla.getNp();
  double ppm = malla.getPpm();
  //Escribir en el archivo np y ppm antes de entrar en el bucle para las partículas
  ofstream output_file(out_file, ios::binary);
  output_file.write(reinterpret_cast<char*>(&ppm), sizeof(ppm));
  output_file.write(reinterpret_cast<char*>(&number_particles), sizeof(number_particles));

  float px_float=0.0;
  float py_float=0.0;
  float pz_float=0.0; //hvx_float=0.0, hvy_float=0.0, hvz_float=0.0, vx_float=0.0, vy_float=0.0, vz_float=0.0;
  float hvx_float=0.0;
  float hvy_float=0.0;
  float hvz_float=0.0;
  float vx_float=0.0;
  float vy_float=0.0;
  float vz_float=0.0;

  vector<Particle> particles_out;
  // Loop through all the blocks
  for (Particle  const& particle: malla.getParticles()){
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



/**
 * Checks if the number of particles is valid.
 * @param np The number of particles to check.
 * @throws runtime_error if the number of particles is less than or equal to zero.
 */
void check_np (int np){
  if (np <= 0) {
    const string errorMsg = "Error: Invalid number of particles: " + to_string(np) + ".\n";
    throw runtime_error(errorMsg);
  }
}







/**
 * Checks if the number of particles in the header matches the number of particles found.
 * @param counter The number of particles found.
 * @param malla_np The number of particles in the header.
 * @throws runtime_error if the number of particles does not match.
 */
void check_missmatch_particles(int counter, int malla_np) {
  if (counter != malla_np) {
    const string errorMsg =
        "Error: Number of particles mismatch. Header: " + to_string(malla_np) + ", Found: " + to_string(counter) +
        ".\n";
    throw runtime_error(errorMsg);
  }
}



void read_particle_field(bool &finished, std::array<int,2> indexes, ifstream &input_file, std::array<std::array<float, 3>, 3> &info_particle) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (!input_file.read(reinterpret_cast<char *>(&info_particle[indexes[0]][indexes[1]]),
                       sizeof(info_particle[indexes[0]][indexes[1]]))) {
    finished = true;

  }
}


