//
// Created by bale2 on 26/09/2023.
//

#include "block.hpp"
#include "progargs.hpp"
#include <iostream>
#include <cmath>


int calculate_block_index(double position, double min_limit, double block_size) {
    // sumamos min_limit para que el bloque 0 sea el bloque que contiene a min_limit
    if (floor ((position - min_limit) / block_size) < 0) {
        return 0;
    }
    return floor((position - min_limit) / block_size);
}
