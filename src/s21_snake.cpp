#include <ncurses.h>

#include "brick_game/snake/controller_s.hpp"
#include "brick_game/snake/model_s.hpp"
#include "gui/cli/gui.hpp"

using namespace s21;

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;

  Snake snake;
  Controller_s controller_s(&snake);

  initscr();
  curs_set(0);
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  snake.current_state_init();
  render(controller_s.updateCurrentState());

  while (snake.state_.status_game) {
    switch (snake.state_.status) {
      case Start:
        updateUserAction(0, snake.state_.time_left, &snake.state_.action);
        controller_s.userInput(snake.state_.action);
        break;
      case Spawn:
        snake.food_init();
        snake.state_.status = Moving;
        break;
      case Moving:
        snake.state_.time_left = snake.process_timer();
        updateUserAction(1, snake.state_.time_left, &snake.state_.action);
        controller_s.userInput(snake.state_.action);
        break;
      case Shifting:
        snake.snake_shifting();
        break;
      case Attaching:
        snake.food_consume();
        break;
      case GameOver:
        controller_s.game_over();
        break;
      case PostGame:
        updateUserAction(0, snake.state_.time_left, &snake.state_.action);
        controller_s.userInput(snake.state_.action);
        break;
      case ExitGame:
        break;
    }
    render(controller_s.updateCurrentState());
  }

  endwin();

  return 0;
}