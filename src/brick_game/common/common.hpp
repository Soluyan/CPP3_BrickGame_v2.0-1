/** @file common.hpp
 * @brief Helper functions for Brickgame.
 */
#ifndef _S21_COMMON_HPP_
#define _S21_COMMON_HPP_

#include "../../structures.h"

int **create_matrix(int H, int W);
void free_matrix(int **matrix, int H);
void clear_matrix(int **matrix, int H, int W);
unsigned long long current_time_millis();

#endif