/**
 * @file model_t.hpp
 * @brief The file contains declarations of the tetris game backend functions
 * and macros.
 */
#ifndef FUNCS_H
#define FUNCS_H

#include "../../structures.h"

#define BACK_INIT() \
  { srand(time(NULL)); }

namespace s21 {
/**
 * @brief class Snake
 */
class Tetris {
 public:
  GameInfo_t state_;
  Tetris() {};
  ~Tetris() = default;

  // GameInfo_t *get_current_state();
  void set_current_state_null();
  void tetris_state_init();
  int **generate_new_block(int *block_size);
  void spawn_block();
  void rotate_block();
  bool can_rotate_block();
  bool can_move_left();
  bool can_move_right();
  bool can_move_down();
  void shifting_block();
  void clear_full_lines();
  void attach_block_to_field();
  void attaching_block();
  unsigned long long process_timer();
  bool is_game_over();

  friend class Controller_t;
};
}  // namespace s21

#endif