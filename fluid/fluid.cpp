#include "sim/progargs.hpp"
#include "sim/block.hpp"
#include <vector>
#include <iostream>

using namespace std;                                /* TODO mala practica?? */



int main(int argc, const char *argv[]) {
    int const error_type = validate_parameters(argc, argv);        /* TODO esto se puede mejorar */
    if (error_type < 0) { return error_type;}
    Malla malla;

    // try catch of read_input_file and any exception
    try {
        malla = read_input_file(argv[2]);
    } catch (const runtime_error& e) {
        cerr << e.what() << "\n";
        return -5;
    }
    write_output_file(malla, argv[3]);
    cout << "el valor de ppm: " << malla.ppm << "\n";
    cout << "el valor de np: " << malla.np << "\n";



    return 0;
}
