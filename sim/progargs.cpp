//
// Created by bale2 on 26/09/2023.
//
#include <iostream>
#include <fstream>
#include "progargs.hpp"

int validate_parameters (int argc, const char * argv[] ){
    using namespace std;

    cout << "este es el parametro 0" << argv[0] << "\n";
    /* Número parametros correctos */
    if(argc != 4){
        cerr << "Error: Invalid number of arguments: " << argc<<"\n";
        return -1;
    }


    /* Primer Argumento es un Número mayor que 0 TODO y el 0??*/
    int nts;
    nts = stoi(argv[1]);
    /* TODO esto no chekea que no sea un numero */

    if (nts == 0){
        cerr << "Error: time steps must be numeric." <<"\n";
        return -1;
    }
    if (nts < 0){
        cerr << "Error: Invalid number of time steps."<<"\n";
        return -2;
    }


    /* Archivo de entrada */
    ifstream input_file(argv[2]);
    if (!input_file.is_open()) {
        cerr << "Error: Cannot open " << argv[2] << " for reading." << "\n";
        return -3;
    }

    /* Verificar si el archivo de salida se puede abrir para escritura */
    ofstream output_file(argv[3]);
    if (!output_file.is_open()) {
        cerr << "Error: Cannot open " << argv[3] << " for writing." << "\n";
        return -4;
    }


    return 0;



    /* Primer Argumento no negativo */

}