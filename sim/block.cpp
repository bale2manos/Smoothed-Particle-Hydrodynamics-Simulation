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

int calculate_block_index(array<int,3> coords,int nx, int ny) {
    return coords[0] + coords[1] * nx + coords[2] * nx * ny;

}


vector<int> check_neighbours(array<int,3> coords, array<int,3> n_blocks){
    vector<int> neighbours;
    // first push back the block itself
    neighbours.push_back(calculate_block_index(coords,n_blocks[0],n_blocks[1]));

    for (int k_aux = -1; k_aux<2; k_aux++){
        for (int j_aux = -1; j_aux<2; j_aux++){
            for (int i_aux = -1; i_aux<2; i_aux++){
              if (i_aux!=0 || j_aux!=0 || k_aux!=0){
                if (coords[0]+i_aux>-1 && coords[0]+i_aux<n_blocks[0] &&
                  coords[1]+j_aux>-1 && coords[1]+j_aux<n_blocks[1] &&
                  coords[2]+k_aux>-1 && coords[2]+k_aux<n_blocks[2]){
                    neighbours.push_back(calculate_block_index({coords[0]+i_aux,coords[1]+j_aux,coords[2]+k_aux},n_blocks[0],n_blocks[1]));
                }
              }
            }
        }
    }
    return neighbours;
}