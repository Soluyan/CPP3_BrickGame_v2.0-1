#include <gtest/gtest.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

#include "../brick_game/common/common.hpp"
#include "../brick_game/snake/controller_s.hpp"
#include "../brick_game/snake/model_s.hpp"
#include "../structures.h"

using namespace s21;

TEST(Snake1, Start1) {
  Snake snake;
  Controller_s controller_s(&snake);
  snake.state_.field = create_matrix(FIELD_H, FIELD_W);
  snake.current_state_init();
  snake.state_.status = Spawn;
  controller_s.userInput(UserStart);
  snake.food_init();
  snake.state_.status = Moving;

  controller_s.userInput(Terminate);
  controller_s.userInput(Terminate);
}

TEST(Snake1, Moves_1) {
  s21::Snake snake;
  s21::Controller_s controller_s(&snake);
  snake.state_.field = create_matrix(FIELD_H, FIELD_W);
  snake.current_state_init();

  controller_s.userInput(UserStart);
  snake.food_init();
  snake.state_.status = Moving;

  snake.state_.food.x = snake.state_.snake[0].x;
  snake.state_.food.y = snake.state_.snake[0].y - 1;

  snake.snake_shifting();

  controller_s.userInput(Down);
  controller_s.userInput(Left);
  controller_s.userInput(Up);
  controller_s.userInput(Right);

  controller_s.userInput(Pause);
  controller_s.userInput(Pause);

  controller_s.userInput(Terminate);
  controller_s.userInput(Terminate);
}

TEST(Snake1, Moves_2) {
  s21::Snake snake;
  s21::Controller_s controller_s(&snake);
  snake.state_.field = create_matrix(FIELD_H, FIELD_W);
  snake.current_state_init();

  controller_s.userInput(UserStart);
  snake.food_init();
  snake.state_.status = Moving;

  controller_s.userInput(Down);
  controller_s.userInput(Left);
  controller_s.userInput(Up);
  controller_s.userInput(Right);

  while (snake.state_.status != GameOver) snake.snake_shifting();

  controller_s.userInput(Terminate);
}

TEST(Snake1, Moves_3) {
  s21::Snake snake;
  s21::Controller_s controller_s(&snake);
  snake.state_.field = create_matrix(FIELD_H, FIELD_W);
  snake.current_state_init();

  controller_s.userInput(UserStart);
  snake.food_init();
  snake.state_.status = Moving;

  controller_s.userInput(Right);
  snake.snake_shifting();
  controller_s.userInput(Down);
  snake.snake_shifting();
  controller_s.userInput(Left);
  snake.snake_shifting();
  controller_s.userInput(Up);
  snake.snake_shifting();

  controller_s.userInput(Terminate);
  controller_s.userInput(Terminate);
}

TEST(Snake1, Moves_4) {
  s21::Snake snake;
  s21::Controller_s controller_s(&snake);
  snake.state_.field = create_matrix(FIELD_H, FIELD_W);
  snake.current_state_init();

  controller_s.userInput(UserStart);
  snake.food_init();
  snake.state_.status = Moving;

  snake.state_.food.x = snake.state_.snake[0].x;
  snake.state_.food.y = snake.state_.snake[0].y - 1;

  snake.snake_shifting();
  snake.food_consume();
  snake.state_.status = Moving;
  snake.state_.time_left = snake.process_timer();

  controller_s.userInput(Down);
  controller_s.userInput(Left);
  controller_s.userInput(Up);
  controller_s.userInput(Right);

  controller_s.userInput(Pause);
  controller_s.userInput(Pause);

  controller_s.userInput(Terminate);
  controller_s.userInput(Terminate);
}

TEST(Snake1, Clear_1) {
  s21::Snake snake;
  s21::Controller_s controller_s(&snake);
  GameInfo_t Game;
  snake.state_.field = create_matrix(FIELD_H, FIELD_W);
  snake.current_state_init();

  Game = controller_s.updateCurrentState();

  controller_s.userInput(UserStart);
  snake.food_init();
  snake.state_.status = Moving;

  snake.state_.time_left = snake.process_timer();

  controller_s.userInput(Terminate);
  controller_s.userInput(Terminate);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}