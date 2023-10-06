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
    float ppm=0;
    input_file.read(reinterpret_cast<char *>(&ppm), sizeof(ppm));
    int np=0;
    input_file.read(reinterpret_cast<char *>(&np), sizeof(np));
    //Comprobamos que np sea mayor que 1
    check_np(np);

    // Calculate constants TODO crear archivo constantes
    // Check if the number of particles read matches the header
    auto ppm_double = static_cast<double>(ppm);
    Constants const constantes = calculate_constants(ppm_double);


    // Creamos la malla y la llenamos de bloques vacíos
    Malla malla = create_fill_grid(np, ppm_double, constantes.nz, constantes.ny, constantes.nx, constantes.h, constantes.m);
    refactor_gordo(in_file, constantes, malla);


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



void refactor_gordo (const char * in_file, Constants cons, Malla malla) {
    ifstream input_file(in_file, ios::binary);
    double trash;
    input_file.read(reinterpret_cast<char *>(&trash), sizeof(double));
    array<array<float, 3>, 3> info_particle;
    array<array<double, 3>, 3> info_particle_double;
    int counter = 0;

    while (input_file.read(reinterpret_cast<char *>(&info_particle[0][0]), sizeof(info_particle[0][0]))) {
        // if i < np then read the next 8 floats, else continue
        input_file.read(reinterpret_cast<char *>(&info_particle[0][1]), sizeof(info_particle[0][1]));
        input_file.read(reinterpret_cast<char *>(&info_particle[0][2]), sizeof(info_particle[0][2]));
        input_file.read(reinterpret_cast<char *>(&info_particle[1][0]), sizeof(info_particle[1][0]));
        input_file.read(reinterpret_cast<char *>(&info_particle[1][1]), sizeof(info_particle[1][1]));
        input_file.read(reinterpret_cast<char *>(&info_particle[1][2]), sizeof(info_particle[1][2]));
        input_file.read(reinterpret_cast<char *>(&info_particle[2][0]), sizeof(info_particle[2][0]));
        input_file.read(reinterpret_cast<char *>(&info_particle[2][1]), sizeof(info_particle[2][1]));
        input_file.read(reinterpret_cast<char *>(&info_particle[2][2]), sizeof(info_particle[2][2]));

        for (size_t i = 0; i < info_particle.size(); ++i) {
            for (size_t j = 0; j < info_particle[i].size(); ++j) {
                info_particle_double[i][j] = static_cast<double>(info_particle[i][j]);
            }
        }

        // check particle inside the grid
        info_particle_double[0] = check_inside_grid(info_particle_double[0]);

        array<int, 3> index_array = calculate_block_indexes(info_particle_double[0], cons);
        // Linear mapping para encontrar el bloque correcto
        int index = index_array[0] + index_array[1] * cons.nx + index_array[2] * cons.nx * cons.ny;
        malla.blocks[index] = insert_particle_info(info_particle_double,malla.blocks[index],counter);


        cout << "Particle " << counter << " Data:" << "\n";
        cout << "px: " << malla.blocks[index].particles.back().p[0] << "\n";
        cout << "py: " << malla.blocks[index].particles.back().p[1] << "\n";
        cout << "pz: " << malla.blocks[index].particles.back().p[2] << "\n";
        cout << "hvx: " << malla.blocks[index].particles.back().hv[0] << "\n";
        cout << "hvy: " << malla.blocks[index].particles.back().hv[1] << "\n";
        cout << "hvz: " << malla.blocks[index].particles.back().hv[2] << "\n";
        cout << "vx: " << malla.blocks[index].particles.back().v[0] << "\n";
        cout << "vy: " << malla.blocks[index].particles.back().v[1] << "\n";
        cout << "vz: " << malla.blocks[index].particles.back().v[2] << "\n";
        cout << "Block: " << malla.blocks[index].i << ", " << malla.blocks[index].j << ", " << malla.blocks[index].k << "\n";
        cout << "PRUEBA" << "\n";
        counter++;
        cout << "counter " << counter;
    }

    missmatch_particles(counter, malla.np);

}


array<double, 3> check_inside_grid(array<double, 3> position){
    if (position[0] < xmin ){position[0] = xmin;}
    if (position[0] > xmax ){position[0] = xmax;}
    if (position[1] < ymin ){position[1] = ymin;}
    if (position[1] > ymax ){position[1] = ymax;}
    if (position[2] < zmin ){position[2] = zmin;}
    if (position[2] > zmax ){position[2] = zmax;}
    return position;
}

array<int, 3> calculate_block_indexes(array <double,3> positions, Constants cons){
    int i,j,k;
    i = initial_block_index(positions[0], xmin,  cons.sx);
    j = initial_block_index(positions[1], ymin,  cons.sy);
    k = initial_block_index(positions[2], zmin,  cons.sz);
    /* TODO problema coma flotante floor error */
    if (i == cons.nx) {        i = cons.nx - 1;       }
    if (j == cons.ny){j = cons.ny - 1;}
    if (k == cons.nz){k = cons.nz - 1;}
    return array<int, 3>{i,j,k};

}

Block insert_particle_info(array<array<double, 3>, 3> info, Block bloque, int id){
    Particle particle;
    particle.p = info[0];
    particle.hv = info[1];
    particle.v = info[2];
    particle.a = {0,g,0};
    particle.rho = 0;
    particle.id = id;
    bloque.particles.emplace_back(particle);
    return bloque;
}

void missmatch_particles(int counter, int malla_np) {
    if (counter != malla_np) {
        string errorMsg =
                "Error: Number of particles mismatch. Header: " + to_string(malla_np) + ", Found: " + to_string(counter) +
                ".\n";
        throw runtime_error(errorMsg);
    }
}



/*
int accinit(Malla malla,  int nts, Constants constants){
    int np = malla.np, i = 0, j = 0, nblocks = malla.blocks.size(), nparticles;
    for (i < nblocks; i++){
        nparticles = malla.blocks[i].particles.size()
                         for (j < nparticles; j++){
            malla.blocks[i].particles[j].ay = g
                                                  malla.blocks[i].particles[j].ds = 0
        }
    }
    return 0;
}
}


int densitycalc(Malla malla, int nts, Constants constants){
    int i = 0, j = 0, k = 0, bol = 0, nblocks = malla.blocks.size(), nparticles;
    float ppm;
    double densij, diffx, diffy, diffz;
    std::vector<Particle> visitedp;
    std::vector<Particle> tovisitb;
    h = Constants.h;

    for(i<nblocks; i++){
        tovisitb::clear()
            nparticles = malla.blocks[i].particles.size();
        int x=malla.blocks[i].i;
        int y=malla.blocks[i].j;
        int z=malla.blocks[i].k;
        tovisitb.push_back(malla.blocks[i]);
        for(int d = 0; d < nblocks; d++){
            if (x+1 == malla.blocks[d].i||x-1==malla.blocks[d].i||y-1==malla.blocks[d].j||y+1==malla.blocks[d].j
                ||z+1==malla.blocks[d].k||z-1==malla.blocks[d].k){
                tovisitb.push_back(malla.blocks[d]);
            }
        }

        for(int j = 0; j< nparticles; j++) {
            visited.push_back(malla.blocks[i].particles[j]);
            for (ii = 0; ii<tovisitb.size; ii++) {
                for (k < nparticles; k++) {
                  bol = 0 for (int l; l < visited.size; l++) {
                    if (malla.blocks[i].particles[k].id == visited[l].id) {
                      bol = 1;
                    }
                  }
                  if bol = 0 {
                      densij = 0;
                      diffx  = malla.blocks[i].particles[j].px -
                              malla.blocks[ii].particles[k].px;
                      diffy = malla.blocks[i].particles[j].py -
                              malla.blocks[ii].particles[k].py;
                      diffz = malla.blocks[i].particles[j].pz -
                              malla.blocks[ii].particles[k].pz;
                      double mod = sqrt(diffx ^ 2 + diffy ^ 2 + diffz ^ 2);
                      if (mod ^ 2 < h ^ 2) { densij = (h ^ 2 - mod ^ 2) ^ 3; }
                      malla.blocks[i].particles[j].rho =
                          densij + malla.blocks[i].particles[j].rho;
                      malla.blocks[i].particles[k].rho =
                          densij + malla.blocks[i].particles[k].rho;
                    }
                }
            }

        }



    }
    return 0;
}


int denstransform(Malla malla, int nts, Constants constants){
    int np = malla.np, i = 0, j = 0, nblocks = malla.blocks.size(), nparticles;
    float ppm;
    h = Constants.h;

    for(i<nblocks; i++){
        nparticles = malla.blocks[i].particles.size();

        for(j< nparticles; j++) {
            malla.blocks[i].particles[j].rho =
                (malla.blocks[i].particles[j].rho + h^6)*(315/(64*M_PI*h^9))* (malla.blocks[i].particles[j].rho/ppm^3)

        }



    }
    return 0;

}


int acceltransf(Malla malla) {
    int i = 0, j = 0, k = 0, bol = 0, nblocks = malla.blocks.size(), nparticles;
    float ppm;
    double densij, diffx, diffy, diffz;
    std::vector<Particle> visitedp;
    std::vector<Particle> tovisitb;
    h = Constants.h;

    for(i<nblocks; i++){
        tovisitb::clear()
            nparticles = malla.blocks[i].particles.size();
        int x=malla.blocks[i].i;
        int y=malla.blocks[i].j;
        int z=malla.blocks[i].k;
        tovisitb.push_back(malla.blocks[i]);
        for(int d = 0; d < nblocks; d++){
            if (x+1 == malla.blocks[d].i||x-1==malla.blocks[d].i||y-1==malla.blocks[d].j||y+1==malla.blocks[d].j
                ||z+1==malla.blocks[d].k||z-1==malla.blocks[d].k){
                tovisitb.push_back(malla.blocks[d]);
            }
        }

        for(int j = 0; j< nparticles; j++) {
            visited.push_back(malla.blocks[i].particles[j]);
            for (ii = 0; ii<tovisitb.size; ii++) {
                for (k < nparticles; k++) {
                  bol = 0 for (int l; l < visited.size; l++) {
                    if (malla.blocks[i].particles[k].id == visited[l].id) {
                      bol = 1;
                    }
                  }
                  if bol = 0 {
                      densij = 0;
                      diffx  = malla.blocks[i].particles[j].px -
                              malla.blocks[ii].particles[k].px;
                      diffy = malla.blocks[i].particles[j].py -
                              malla.blocks[ii].particles[k].py;
                      diffz = malla.blocks[i].particles[j].pz -
                              malla.blocks[ii].particles[k].pz;
                      double mod = sqrt(diffx ^ 2 + diffy ^ 2 + diffz ^ 2);
                      if (mod ^ 2 < h ^ 2) {
                        accelx = (diffx * (15/M_PI*h^6) * (malla.blocks[i].particles[j].rho/ppm^3)^2 *((h-distij)^2/distij) * (
                                                                                                                                                     malla.blocks[i].particles[j].rho + malla.blocks[ii].particles[k].rho - 2 * rho_f)
                                                                                                                   + (malla.blocks[i].particles[j].vx + malla.blocks[ii].particles[j].vx) * (45/M_PI*h^6) * mu)/(
                                     malla.blocks[i].particles[j].rho*malla.blocks[ii].particles[k].rho) ;
                        accely = (diffy * (15/M_PI*h^6) * (malla.blocks[i].particles[j].rho/ppm^3)^2 *((h-distij)^2/distij) * (
                                                                                                                                                     malla.blocks[i].particles[j].rho + malla.blocks[ii].particles[k].rho - 2 * rho_f)
                                                                                                                   + (malla.blocks[i].particles[j].vy + malla.blocks[ii].particles[j].vy) * (45/M_PI*h^6) * mu)/(
                                     malla.blocks[i].particles[j].rho*malla.blocks[ii].particles[k].rho) ;
                        accelz = (diffz * (15/M_PI*h^6) * (malla.blocks[i].particles[j].rho/ppm^3)^2 *((h-distij)^2/distij) * (
                                                                                                                                                     malla.blocks[i].particles[j].rho + malla.blocks[ii].particles[k].rho - 2 * rho_f)
                                                                                                                   + (malla.blocks[i].particles[j].vz + malla.blocks[ii].particles[j].vz) * (45/M_PI*h^6) * mu)/(
                                     malla.blocks[i].particles[j].rho*malla.blocks[ii].particles[k].rho) ;
                      }
                      malla.blocks[i].particles[j].ax =
                          accelx + malla.blocks[i].particles[j].ax;
                      malla.blocks[i].particles[k].ax =
                          accelx + malla.blocks[i].particles[k].ax;
                      malla.blocks[i].particles[j].ay =
                          accely + malla.blocks[i].particles[j].ay;
                      malla.blocks[i].particles[k].ay =
                          accely + malla.blocks[i].particles[k].ay;
                      malla.blocks[i].particles[j].ay =
                          accelz + malla.blocks[i].particles[j].az;
                      malla.blocks[i].particles[k].az =
                          accelz + malla.blocks[i].particles[k].az;
                    }
                }
            }

        }



    }
    return 0;
}


}