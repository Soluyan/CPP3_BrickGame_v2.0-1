#include <ncurses.h>
#include <stdlib.h>

#include "brick_game/common/common.hpp"
#include "brick_game/tetris/controller_t.hpp"
#include "brick_game/tetris/model_t.hpp"
#include "gui/cli/gui.hpp"

using namespace s21;

int main(int argc, char *argv[]) {
  Tetris tetris;
  Controller_t controller_t(&tetris);
  (void)argc;
  (void)argv;
  initscr();
  curs_set(0);
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  tetris.tetris_state_init();

  GameInfo_t Game_state = controller_t.updateCurrentState();
  render(Game_state);
  free_matrix(Game_state.field, FIELD_H);

  while (tetris.state_.status_game) {
    switch (tetris.state_.status) {
      case Start:
        updateUserAction(0, tetris.state_.time_left, &tetris.state_.action);
        controller_t.userInput(tetris.state_.action);
        break;
      case Spawn:
        tetris.spawn_block();
        break;
      case Moving:
        tetris.state_.time_left = tetris.process_timer();
        updateUserAction(1, tetris.state_.time_left, &tetris.state_.action);
        controller_t.userInput(tetris.state_.action);
        break;
      case Shifting:
        break;
      case Attaching:
        tetris.attaching_block();
        break;
      case GameOver:
        controller_t.game_over();
        break;
      case PostGame:
        updateUserAction(0, tetris.state_.time_left, &tetris.state_.action);
        controller_t.userInput(tetris.state_.action);
        break;
      case ExitGame:
        break;
    }
    Game_state = controller_t.updateCurrentState();
    render(Game_state);
    free_matrix(Game_state.field, FIELD_H);
  }
  controller_t.cleanup_game();

  endwin();
  return 0;
}