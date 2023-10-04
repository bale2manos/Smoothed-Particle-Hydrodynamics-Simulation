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
        cout << "position: " << position << "\n";
        cout << "min_limit: " << min_limit << "\n";
        cout << "block_size: " << block_size << "\n";
    }
    return floor((position - min_limit) / block_size);
}
