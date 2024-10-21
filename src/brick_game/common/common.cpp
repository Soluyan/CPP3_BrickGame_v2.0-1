/** @file common.hpp
 * @brief Helper functions for Brickgame.
 */
#include "common.hpp"

#include <stdlib.h>
#include <sys/time.h>

/**
 * @brief Creates a square matrix.
 * @param H Matrix height parameter.
 * @param W Matrix width parameter.
 * @return Returns an empty matrix with dimensions given by H and W;
 */
int **create_matrix(int H, int W) {
  int **matrix = new int *[H];
  if (matrix == NULL) {
    return NULL;
  }
  for (int i = 0; i < H; i++) {
    matrix[i] = new int[W]();
    if (matrix[i] == NULL) {
      for (int j = 0; j < i; j++) {
        delete[] matrix[j];
      }
      delete[] matrix;
      return NULL;
    }
  }
  return matrix;
}

/**
 * @brief Clears the square matrix.
 * @param[in,out] matrix The pointer to the Matrix for purification.
 * @param H Matrix height parameter.
 */
void free_matrix(int **matrix, int H) {
  if (matrix != NULL) {
    for (int i = 0; i < H; i++) {
      delete[] matrix[i];
    }
    delete[] matrix;
  }
}

/**
 * @brief Sets all elements in a square matrix to zero.
 * @param[in,out] matrix The pointer to which the Matrix for resetting is
 * located.
 * @param H Matrix height parameter.
 * @param W Matrix width parameter.
 */
void clear_matrix(int **matrix, int H, int W) {
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      matrix[i][j] = 0;
    }
  }
}

/**
 * @brief calculates the current time.
 * @return Return current time;
 */
unsigned long long current_time_millis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (unsigned long long)(tv.tv_sec) * 1000 +
         (unsigned long long)(tv.tv_usec) / 1000;
}