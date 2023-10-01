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

    if (np <= 0) {
        cerr << "Error: Invalid number of particles:"<< np << ".\n";
        return -5;
    }

    // Print the values to the screen
    cout << "ppm: " << ppm << endl;
    cout << "np: " << np << endl;

    vector<Particle> particles(np);                         /*Array de estructuras*/


    // Read particle data in a single loop and cast from float to double
    float px_float, py_float, pz_float, hvx_float, hvy_float, hvz_float, vx_float, vy_float, vz_float;
    int i = 0;
    while (!input_file.eof()) {
        input_file.read(reinterpret_cast<char*>(&px_float), sizeof(px_float));
        input_file.read(reinterpret_cast<char*>(&py_float), sizeof(py_float));
        input_file.read(reinterpret_cast<char*>(&pz_float), sizeof(pz_float));
        input_file.read(reinterpret_cast<char*>(&hvx_float), sizeof(hvx_float));
        input_file.read(reinterpret_cast<char*>(&hvy_float), sizeof(hvy_float));
        input_file.read(reinterpret_cast<char*>(&hvz_float), sizeof(hvz_float));
        input_file.read(reinterpret_cast<char*>(&vx_float), sizeof(vx_float));
        input_file.read(reinterpret_cast<char*>(&vy_float), sizeof(vy_float));
        input_file.read(reinterpret_cast<char*>(&vz_float), sizeof(vz_float));


        // Cast from float to double and store in the Particle struct
        particles[i].px = static_cast<double>(px_float);
        particles[i].py = static_cast<double>(py_float);
        particles[i].pz = static_cast<double>(pz_float);
        particles[i].hvx = static_cast<double>(hvx_float);
        particles[i].hvy = static_cast<double>(hvy_float);
        particles[i].hvz = static_cast<double>(hvz_float);
        particles[i].vx = static_cast<double>(vx_float);
        particles[i].vy = static_cast<double>(vy_float);
        particles[i].vz = static_cast<double>(vz_float);

        i++; // Increment the counter
    }


    if (i-1 != np) {
        cerr << "Error: Number of particles mismatch. Header: " << np << ", Found: " << i << ".\n";
        return -5;
    }

    for (int k=0; k<np; k++) {
        cout << "Particle " << k << " Data:" << "\n";
        cout << "px: " << particles[k].px << "\n";
        cout << "py: " << particles[k].py << "\n";
        cout << "pz: " << particles[k].pz << "\n";
        cout << "hvx: " << particles[k].hvx << "\n";
        cout << "hvy: " << particles[k].hvy << "\n";
        cout << "hvz: " << particles[k].hvz << "\n";
        cout << "vx: " << particles[k].vx << "\n";
        cout << "vy: " << particles[k].vy << "\n";
        cout << "vz: " << particles[k].vz << "\n";
        cout << "\n";
    }
    input_file.close(); /* TODO esto hay que cerrarlo? */

    // Check if the number of particles read matches the header
    cout << "i: " << i-1 << "\n";
    cout << "np: " << np << "\n";


    return 0;
}
