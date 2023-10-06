//
// Created by bale2 on 26/09/2023.
//

#include "block.hpp"
#include "progargs.hpp"
#include <iostream>
#include <cmath>


int initial_block_index(double position, double min_limit, double block_size) {
    // sumamos min_limit para que el bloque 0 sea el bloque que contiene a min_limit
    return floor((position - min_limit) / block_size);
}

int calculate_block_index(int i,int j, int k,int nx, int ny) {
    return i + j * nx + k * nx * ny;

}


vector<int> check_neighbours(int i, int j, int k, double nx, double ny, double nz){
    vector<int> neighbours;
    for (int i_aux = 1; i_aux>-2; i_aux--){
        for (int j_aux = 1; j_aux>-2; j_aux--){
            for (int k_aux = 1; k_aux>-2; k_aux--){
                if (i+i_aux>-1 && i+i_aux<nx && j+j_aux>-1 && j+j_aux<ny && k+k_aux>-1 && k+k_aux<nz){
                    neighbours.push_back(calculate_block_index(i+i_aux,j+j_aux,k+k_aux,nx,ny));
                }
            }
        }
    }
    return neighbours;
}