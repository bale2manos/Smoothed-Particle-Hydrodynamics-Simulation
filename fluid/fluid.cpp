#include "sim/progargs.hpp"
#include "sim/block.hpp"
#include <vector>
#include <iostream>

using namespace std;                                /* TODO mala practica?? */



int main(int argc, const char *argv[]) {
    vector<string> const args(argv, argv + argc);
    int const error_type = validate_parameters(argc, argv);        /* TODO esto se puede mejorar */
    if (error_type < 0) { return error_type;}
    Malla malla;

    // try catch of read_input_file and any exception
    try {
        malla = read_input_file(args[2].c_str());
    } catch (const runtime_error& e) {
        cerr << e.what() << "\n";
        return -5;
    }

    cuatropunto3punto2 (malla);
    changeaccel(malla);




    write_output_file(malla, argv[3]);




    return 0;
}
