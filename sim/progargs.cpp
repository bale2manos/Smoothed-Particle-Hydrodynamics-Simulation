#include <iostream>
#include <fstream>
#include <string>
#include "progargs.hpp"



// Define a struct to represent a particle
struct Particle {
    double px, py, pz, hvx, hvy, hvz, vx, vy, vz;
};
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
    ifstream input_file(inputFileName);
    if (!input_file.is_open()) {
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

int validate_parameters(int argc, const char* argv[]) {

    if (argc != 4) {
        cerr << "Error: Invalid number of arguments: " << argc << "\n";
        return -1;
    }

    int nts = stoi(argv[1]);
    if (validate_time_steps(nts) != 0) return -1;
    if (validate_input_file(argv[2]) != 0) return -3;
    if (validate_output_file(argv[3]) != 0)return -4;


    return 0;
}

int read_input_file (const char * in_file){
    ifstream input_file(in_file, ios::binary);
    float ppm;                                                              /* TODO ppm check errors? */
    input_file.read(reinterpret_cast<char*>(&ppm), sizeof(ppm));

    int np;
    input_file.read(reinterpret_cast<char*>(&np), sizeof(np));

    if (np <= 0) {
        cerr << "Error: Invalid number of particles:"<< np << ".\n";
        return -5;
    }

    // Print the values to the screen
    cout << "ppm: " << ppm << endl;
    cout << "np: " << np << endl;

    vector<Particle> particles;                         /*Array de estructuras*/

    // Read particle data in a single loop and cast from float to double
    float px_float, py_float, pz_float, hvx_float, hvy_float, hvz_float, vx_float, vy_float, vz_float;
    int i = 0;
    while (input_file.read(reinterpret_cast<char*>(&px_float), sizeof(px_float))) {
        // if i < np then read the next 8 floats, else continue
        input_file.read(reinterpret_cast<char*>(&py_float), sizeof(py_float));
        input_file.read(reinterpret_cast<char*>(&pz_float), sizeof(pz_float));
        input_file.read(reinterpret_cast<char*>(&hvx_float), sizeof(hvx_float));
        input_file.read(reinterpret_cast<char*>(&hvy_float), sizeof(hvy_float));
        input_file.read(reinterpret_cast<char*>(&hvz_float), sizeof(hvz_float));
        input_file.read(reinterpret_cast<char*>(&vx_float), sizeof(vx_float));
        input_file.read(reinterpret_cast<char*>(&vy_float), sizeof(vy_float));
        input_file.read(reinterpret_cast<char*>(&vz_float), sizeof(vz_float));


        // Cast from float to double and store in the Particle struct
        particles.emplace_back();
        particles[i].px = static_cast<double>(px_float);
        particles[i].py = static_cast<double>(py_float);
        particles[i].pz = static_cast<double>(pz_float);
        particles[i].hvx = static_cast<double>(hvx_float);
        particles[i].hvy = static_cast<double>(hvy_float);
        particles[i].hvz = static_cast<double>(hvz_float);
        particles[i].vx = static_cast<double>(vx_float);
        particles[i].vy = static_cast<double>(vy_float);
        particles[i].vz = static_cast<double>(vz_float);

        cout << "Particle " << i << " Data:" << "\n";
        cout << "px: " << particles[i].px << "\n";
        cout << "py: " << particles[i].py << "\n";
        cout << "pz: " << particles[i].pz << "\n";
        cout << "hvx: " << particles[i].hvx << "\n";
        cout << "hvy: " << particles[i].hvy << "\n";
        cout << "hvz: " << particles[i].hvz << "\n";
        cout << "vx: " << particles[i].vx << "\n";
        cout << "vy: " << particles[i].vy << "\n";
        cout << "vz: " << particles[i].vz << "\n";
        cout << "\n";

        i++; // Increment the counter
    }


    if (i != np) {
        cerr << "Error: Number of particles mismatch. Header: " << np << ", Found: " << i << ".\n";
        return -5;
    }

    input_file.close(); /* TODO esto hay que cerrarlo? */

    // Check if the number of particles read matches the header
    double h = smooth_length(ppm);
    double xmax = 0.065;double xmin = -0.065;double ymax = 0.1;double ymin = -0.08;double zmax = 0.065;double zmin = -0.065;
    double nx = nx_calc(xmax, xmin, h);
    double ny = ny_calc(ymax, ymin, h);
    double nz = nz_calc(zmax, zmin, h);
    double sx = sx_calc(xmax, xmin, nx);
    double sy = sy_calc(ymax, ymin, ny);
    double sz = sz_calc(zmax, zmin, nz);


    cout << "Number of particles: " << np << "\n";
    cout << "Particles per meter: " << ppm << "\n";
    cout << "Smoothing length: " << h << "\n";
    cout << "Particle mass: " << particle_mass(ppm) << "\n";
    cout << "Grid size: " << nx << " x "<< ny << " x " << nz << "\n";
    cout << "Number of blocks: " << nx*ny*nz << "\n";
    cout << "Block size: " << sx << " x " << sy << " x " << sz << "\n";


    return 0;
}

double smooth_length (double ppm){
    return static_cast<double>(1.695) / ppm;
}
double particle_mass (double ppm){
    return static_cast<double>(pow (10,3))/(pow (ppm,3));
}
double nx_calc (double xmax, double xmin, double h){
    return floor( (xmax - xmin) / h);
}
double ny_calc (double ymax, double ymin, double h){
    return floor( (ymax - ymin) / h);
}
double nz_calc (double zmax, double zmin, double h){
    return floor( (zmax - zmin) / h);
}


double sx_calc (double xmax, double xmin, double nx){
    return (xmax-xmin)/nx;
}
double sy_calc (double ymax, double ymin, double ny){
    return (ymax-ymin)/ny;
}double sz_calc (double zmax, double zmin, double nz){
    return (zmax-zmin)/nz;
}
