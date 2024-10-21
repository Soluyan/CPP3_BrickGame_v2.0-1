/**
 * @file model_s.hpp
 * @brief The file contains declarations of the snake game structures, all its
 * backend functions and macros.
 */
#ifndef _S21_SNAKE_HPP_
#define _S21_SNAKE_HPP_

#include "../../structures.h"

/**
 * @brief number of figure orientations
 */
#define ORI 4

/**
 * @brief initial number of snake blocks
 */
#define WORMSIZE 4

namespace s21 {
/**
 * @brief class Snake
 */
class Snake {
 public:
  GameInfo_t state_;
  Snake() {};
  ~Snake();

  //   GameInfo_t *get_current_state();
  void current_state_init();
  int import_score();
  void export_score();
  void food_init();
  bool snake_collision(snake_orientation orientation, snake_part snake_head,
                       snake_part element);
  bool tail_collision();
  void snake_init();
  void snake_drawing();
  void tail_shift();
  bool field_collision();
  void food_consume();
  void snake_move();
  void snake_shifting();
  unsigned long long process_timer();
  void exit_with_error();

 private:
};

}  // namespace s21

#endif