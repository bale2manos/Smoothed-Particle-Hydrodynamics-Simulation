#include <iostream>
#include "../sim/progargs.cpp"                      /* TODO esto como funciona? */
#include <vector>
using namespace std;                                /* TODO mala practica?? */

// Define a struct to represent a particle
struct Particle {
    double px, py, pz, hvx, hvy, hvz, vx, vy, vz;
};

int main(int argc, const char * argv[] ) {

    int error_type = validate_parameters(argc, argv);        /* TODO esto se puede mejorar */
    if (error_type < 0) return error_type;
    ifstream input_file(argv[2], ios::binary);

    float ppm;                                                              /* TODO ppm check errors? */
    input_file.read(reinterpret_cast<char*>(&ppm), sizeof(ppm));

    int np;
    input_file.read(reinterpret_cast<char*>(&np), sizeof(np));

    if np <= 0 {
        cerr << "Error: Invalid number of particles:"<< np << ".\n";
        return -5;
    }

    // Print the values to the screen
    cout << "ppm: " << ppm << endl;
    cout << "np: " << np << endl;

    vector<Particle> particles(np);                         /*Array de estructuras*/

    for (int i = 0; i < np; ++i) {
        input_file.read(reinterpret_cast<char*>(&particles[i].px), sizeof(particles[i].px));
        input_file.read(reinterpret_cast<char*>(&particles[i].py), sizeof(particles[i].py));
        input_file.read(reinterpret_cast<char*>(&particles[i].pz), sizeof(particles[i].pz));
        input_file.read(reinterpret_cast<char*>(&particles[i].hvx), sizeof(particles[i].hvx));
        input_file.read(reinterpret_cast<char*>(&particles[i].hvy), sizeof(particles[i].hvy));
        input_file.read(reinterpret_cast<char*>(&particles[i].hvz), sizeof(particles[i].hvz));
        input_file.read(reinterpret_cast<char*>(&particles[i].vx), sizeof(particles[i].vx));
        input_file.read(reinterpret_cast<char*>(&particles[i].vy), sizeof(particles[i].vy));
        input_file.read(reinterpret_cast<char*>(&particles[i].vz), sizeof(particles[i].vz));
    }

    for (int k=0; k<10; k++) {
        cout << "Particle " << k << " Data:" << std::endl;
        cout << "px: " << particles[k].px << std::endl;
        cout << "py: " << particles[k].py << std::endl;
        cout << "pz: " << particles[k].pz << std::endl;
        cout << "hvx: " << particles[k].hvx << std::endl;
        cout << "hvy: " << particles[k].hvy << std::endl;
        cout << "hvz: " << particles[k].hvz << std::endl;
        cout << "vx: " << particles[k].vx << std::endl;
        cout << "vy: " << particles[k].vy << std::endl;
        cout << "vz: " << particles[k].vz << std::endl;
        cout << std::endl;
    }
    input_file.close(); /* TODO esto hay que cerrarlo? */



    return 0;
}
