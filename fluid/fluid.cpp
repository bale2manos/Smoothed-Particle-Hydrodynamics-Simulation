#include "../sim/progargs.cpp"
using namespace std;                                /* TODO mala practica?? */



int main(int argc, const char * argv[] ) {

    int error_type = validate_parameters(argc, argv);        /* TODO esto se puede mejorar */
    if (error_type < 0) return error_type;
    int j = read_input_file(argv[2]);


    return 0;
}
