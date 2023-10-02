#include "sim/progargs.hpp"

using namespace std;                                /* TODO mala practica?? */



int main(int argc, const char *argv[]) {
    int const error_type = validate_parameters(argc, argv);        /* TODO esto se puede mejorar */
    if (error_type < 0) { return error_type; }
    read_input_file(argv[2]);


    return 0;
}
