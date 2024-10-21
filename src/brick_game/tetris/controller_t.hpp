/** @file controller_t.hpp
 * @brief backend for tetris control functions.
 */
#ifndef _S21_CONTROLLER_T_HPP_
#define _S21_CONTROLLER_T_HPP_

#include "model_t.hpp"

namespace s21 {
/**
 * @brief class controller
 */
class Controller_t {
 private:
  Tetris *tetris;

 public:
  Controller_t(Tetris *t) : tetris(t) {};
  ~Controller_t() = default;

  GameInfo_t updateCurrentState();
  void userInput(UserAction_t action);
  void pause_game();
  void game_over();
  void terminate_game();
  void cleanup_game();
};

}  // namespace s21

#endif