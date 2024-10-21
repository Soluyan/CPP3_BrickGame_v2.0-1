/** @file controller_s.hpp
 * @brief backend for snake control functions.
 */
#ifndef _S21_CONTROLLER_HPP_
#define _S21_CONTROLLER_HPP_

#include "model_s.hpp"
namespace s21 {
/**
 * @brief class controller
 */
class Controller_s {
 public:
  Snake *snake;

  Controller_s(Snake *s) : snake(s) {};
  ~Controller_s() = default;

  GameInfo_t updateCurrentState();
  void userInput(UserAction_t action);
  void pause_game();
  void game_over();
  void terminate_game();

 private:
};

}  // namespace s21

#endif