#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "progargs.hpp"
#include "block.hpp"
#include "grid.hpp"
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

Malla read_input_file (const char * in_file) {
    ifstream input_file(in_file, ios::binary);     /* TODO ppm check errors? */

    // Crear la malla base
    float ppm;
    input_file.read(reinterpret_cast<char *>(&ppm), sizeof(ppm));
    int np;
    input_file.read(reinterpret_cast<char *>(&np), sizeof(np));

    // Calculate constants TODO crear archivo constantes
    // Check if the number of particles read matches the header
    double const h = smooth_length(ppm);
    double const xmax = 0.065;
    double const xmin = -0.065;
    double const ymax = 0.1;
    double const ymin = -0.08;
    double const zmax = 0.065;
    double const zmin = -0.065;
    double const nx = nx_calc(xmax, xmin, h);
    double const ny = ny_calc(ymax, ymin, h);
    double const nz = nz_calc(zmax, zmin, h);
    double const sx = sx_calc(xmax, xmin, nx);
    double const sy = sy_calc(ymax, ymin, ny);
    double const sz = sz_calc(zmax, zmin, nz);


    if (np <= 0) {
        string errorMsg = "Error: Invalid number of particles: " + to_string(np) + ".\n";
        throw runtime_error(errorMsg);
    }

    // Print the values to the screen
    cout << "ppm: " << ppm << "\n";
    cout << "np: " << np << "\n";

    int i, j, k;
    Malla malla(np, ppm, vector<Block>());
    // Create the blocks and append them to Malla. Create all vectors in nx, ny, nz
    for (k = 0; k < nz; ++k) {
        for (j = 0; j < ny; ++j) {
            for (i = 0; i < nx; ++i) {
                malla.blocks.push_back(Block(i, j, k, vector<Particle>()));
            }
        }
    }
    // Read particle data in a single loop and cast from float to double
    float px_float, py_float, pz_float, hvx_float, hvy_float, hvz_float, vx_float, vy_float, vz_float;
    int counter = 0;
    while (input_file.read(reinterpret_cast<char *>(&px_float), sizeof(px_float))) {
        // if i < np then read the next 8 floats, else continue
        input_file.read(reinterpret_cast<char *>(&py_float), sizeof(py_float));
        input_file.read(reinterpret_cast<char *>(&pz_float), sizeof(pz_float));
        input_file.read(reinterpret_cast<char *>(&hvx_float), sizeof(hvx_float));
        input_file.read(reinterpret_cast<char *>(&hvy_float), sizeof(hvy_float));
        input_file.read(reinterpret_cast<char *>(&hvz_float), sizeof(hvz_float));
        input_file.read(reinterpret_cast<char *>(&vx_float), sizeof(vx_float));
        input_file.read(reinterpret_cast<char *>(&vy_float), sizeof(vy_float));
        input_file.read(reinterpret_cast<char *>(&vz_float), sizeof(vz_float));


        i = calculate_block_index(static_cast<double>(px_float), xmin, sx);
        j = calculate_block_index(static_cast<double>(py_float), ymin, sy);
        k = calculate_block_index(static_cast<double>(pz_float), zmin, sz);
        if (i >= nx){i= nx-1;}
        if (j >= ny){j= ny-1;}
        if (k >= nz){k= nz-1;}

        // Linear mapping para encontrar el bloque correcto
        int index = i + j * nx + k * nx * ny;
        Block &selectedBlock = malla.blocks[index];
        selectedBlock.particles.emplace_back();
        selectedBlock.particles.back().id = counter;
        selectedBlock.particles.back().px = static_cast<double>(px_float);
        selectedBlock.particles.back().py = static_cast<double>(py_float);
        selectedBlock.particles.back().pz = static_cast<double>(pz_float);
        selectedBlock.particles.back().hvx = static_cast<double>(hvx_float);
        selectedBlock.particles.back().hvy = static_cast<double>(hvy_float);
        selectedBlock.particles.back().hvz = static_cast<double>(hvz_float);
        selectedBlock.particles.back().vx = static_cast<double>(vx_float);
        selectedBlock.particles.back().vy = static_cast<double>(vy_float);
        selectedBlock.particles.back().vz = static_cast<double>(vz_float);

        cout << "Particle " << counter << " Data:" << "\n";
        cout << "px: " << selectedBlock.particles.back().px << "\n";
        cout << "py: " << selectedBlock.particles.back().py << "\n";
        cout << "pz: " << selectedBlock.particles.back().pz << "\n";
        cout << "hvx: " << selectedBlock.particles.back().hvx << "\n";
        cout << "hvy: " << selectedBlock.particles.back().hvy << "\n";
        cout << "hvz: " << selectedBlock.particles.back().hvz << "\n";
        cout << "vx: " << selectedBlock.particles.back().vx << "\n";
        cout << "vy: " << selectedBlock.particles.back().vy << "\n";
        cout << "vz: " << selectedBlock.particles.back().vz << "\n";
        cout << "\n";
        counter++; // Increment the counter
    }

    if (counter != np) {
        string errorMsg =
                "Error: Number of particles mismatch. Header: " + to_string(np) + ", Found: " + to_string(counter) +
                ".\n";
        throw runtime_error(errorMsg);
    }

    input_file.close(); /* TODO esto hay que cerrarlo? */

    cout << "Number of particles: " << np << "\n";
    cout << "Particles per meter: " << ppm << "\n";
    cout << "Smoothing length: " << h << "\n";
    cout << "Particle mass: " << particle_mass(ppm) << "\n";
    cout << "Grid size: " << nx << " x " << ny << " x " << nz << "\n";
    cout << "Number of blocks: " << nx * ny * nz << "\n";
    cout << "Block size: " << sx << " x " << sy << " x " << sz << "\n";


    return malla;
}


int write_output_file (Malla malla, const char * out_file){
    int np = malla.np;
    float ppm = malla.ppm;
    //Escribir en el archivo np y ppm antes de entrar en el bucle para las partículas
    ofstream output_file(out_file, ios::binary);
    output_file.write(reinterpret_cast<char*>(&ppm), sizeof(ppm));
    output_file.write(reinterpret_cast<char*>(&np), sizeof(np));

    float px_float, py_float, pz_float, hvx_float, hvy_float, hvz_float, vx_float, vy_float, vz_float;
    vector<Particle> particles_out;
    // Loop through all the blocks
    for (Block & block : malla.blocks) {
        // Loop through all the particles in the block
        for (Particle & particle : block.particles) {
            // Cast from double to float and write to file
            particles_out.push_back(particle);
        }
    }
    // Sort particles_out by id
    sort(particles_out.begin(), particles_out.end(), [](Particle & a, Particle & b) { return a.id < b.id; });

    for (Particle & particle : particles_out) {
        px_float = static_cast<float>(particle.px);
        py_float = static_cast<float>(particle.py);
        pz_float = static_cast<float>(particle.pz);
        hvx_float = static_cast<float>(particle.hvx);
        hvy_float = static_cast<float>(particle.hvy);
        hvz_float = static_cast<float>(particle.hvz);
        vx_float = static_cast<float>(particle.vx);
        vy_float = static_cast<float>(particle.vy);
        vz_float = static_cast<float>(particle.vz);
        output_file.write(reinterpret_cast<char*>(&px_float), sizeof(px_float));
        output_file.write(reinterpret_cast<char*>(&py_float), sizeof(py_float));
        output_file.write(reinterpret_cast<char*>(&pz_float), sizeof(pz_float));
        output_file.write(reinterpret_cast<char*>(&hvx_float), sizeof(hvx_float));
        output_file.write(reinterpret_cast<char*>(&hvy_float), sizeof(hvy_float));
        output_file.write(reinterpret_cast<char*>(&hvz_float), sizeof(hvz_float));
        output_file.write(reinterpret_cast<char*>(&vx_float), sizeof(vx_float));
        output_file.write(reinterpret_cast<char*>(&vy_float), sizeof(vy_float));
        output_file.write(reinterpret_cast<char*>(&vz_float), sizeof(vz_float));
    }

    output_file.close();

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
}

double sz_calc (double zmax, double zmin, double nz){
    return (zmax-zmin)/nz;
}



