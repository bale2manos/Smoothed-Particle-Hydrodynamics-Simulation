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
    Constants constantes = calculate_constants(ppm);
    //Comprobamos que np sea mayor que 1
    check_np(np);
    // Creamos la malla y la llenamos de bloques vacíos
    Malla malla = create_fill_grid(np, ppm, constantes.nz, constantes.ny, constantes.nx);


    refactor_gordo(input_file);








    // Read particle data in a single loop and cast from float to double

    input_file.close(); /* TODO esto hay que cerrarlo? */

    cout << "Number of particles: " << np << "\n";
    cout << "Particles per meter: " << ppm << "\n";
    cout << "Smoothing length: " << constantes.h << "\n";
    cout << "Particle mass: " << particle_mass(ppm) << "\n";
    cout << "Grid size: " << constantes.nx << " x " << constantes.ny << " x " << constantes.nz << "\n";
    cout << "Number of blocks: " <<  constantes.nx *  constantes.ny *  constantes.nz << "\n";
    cout << "Block size: " << constantes.sx << " x " << constantes.sy << " x " << constantes.sz << "\n";


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



void check_np (int np){
    if (np <= 0) {
        string errorMsg = "Error: Invalid number of particles: " + to_string(np) + ".\n";
        throw runtime_error(errorMsg);
    }
}



void refactor_gordo (ifstream input_file) {

    float px_float, py_float, pz_float, hvx_float, hvy_float, hvz_float, vx_float, vy_float, vz_float;
    int counter = 0;
    vector<float> buffer(10);
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


        // Check if the particle is inside the domain
        if (px_float < xmin ){px_float = xmin;}
        if (px_float > xmax ){px_float = xmax;}
        if (py_float < ymin ){py_float = ymin;}
        if (py_float > ymax ){py_float = ymax;}
        if (pz_float < zmin ){pz_float = zmin;}
        if (pz_float > zmax ){pz_float = zmax;}

        int i,j,k;
        i = calculate_block_index(static_cast<double>(px_float), xmin,  constantes.sx);
        j = calculate_block_index(static_cast<double>(py_float), ymin,  constantes.sy);
        k = calculate_block_index(static_cast<double>(pz_float), zmin,  constantes.sz);

        // Linear mapping para encontrar el bloque correcto
        int index = i + j * constantes.nx + k * constantes.nx * constantes.ny;
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
        selectedBlock.particles.back().ax = 0;
        selectedBlock.particles.back().ay = g;
        selectedBlock.particles.back().az = 0;
        selectedBlock.particles.back().rho = 0;

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
        cout << "Block: " << selectedBlock.i << ", " << selectedBlock.j << ", " << selectedBlock.k << "\n";
        counter++; // Increment the counter
    }



    if (counter != np) {
        string errorMsg =
                "Error: Number of particles mismatch. Header: " + to_string(np) + ", Found: " + to_string(counter) +
                ".\n";
        throw runtime_error(errorMsg);
    }

}

