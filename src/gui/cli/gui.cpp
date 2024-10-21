/** @file gui.cpp
 * @brief main func for terminal version
 */
#include "gui.hpp"

#include <ncurses.h>

#include "../../brick_game/snake/controller_s.hpp"
#include "../../brick_game/snake/model_s.hpp"
#include "../../brick_game/tetris/controller_t.hpp"
#include "../../brick_game/tetris/model_t.hpp"

using namespace s21;

/**
 * @brief drawing game state using library curses.h.
 * @param[in] info state of the game
 */
void render(GameInfo_t info) {
  WINDOW *game_window = newwin(FIELD_H + 2, 2 * FIELD_W + 2, 0, 0);
  box(game_window, 0, 0);
  refresh();
  wrefresh(game_window);

  // Render game field
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      if (info.field[i][j]) {
        mvwprintw(game_window, i + 1, 2 * j + 1, "[]");
      } else {
        mvwprintw(game_window, i + 1, 2 * j + 1, "  ");
      }
    }
  }
  wrefresh(game_window);

  // Create the side panel
  WINDOW *side_panel = newwin(FIELD_H + 2, 20, 0, 2 * FIELD_W + 2);
  refresh();
  box(side_panel, 0, 0);

  // Render side panel
  mvwprintw(side_panel, 1, 1, "Pause: %s", info.pause ? "Yes" : "NO");
  if (info.status == Start) mvwprintw(side_panel, 15, 1, "Press start button");
#ifdef SNAKE_CLI
  if (info.status == PostGame && info.score >= 196) {
    mvwprintw(side_panel, 14, 1, "Congratulations!");
    mvwprintw(side_panel, 15, 1, "you won.");
  } else if (info.status == PostGame) {
    mvwprintw(side_panel, 14, 1, "Game over! Press");
    mvwprintw(side_panel, 15, 1, "start button or");
    mvwprintw(side_panel, 16, 1, "Exit button");
  }
#endif

#ifdef TETRIS_CLI
  if (info.status == PostGame) {
    mvwprintw(side_panel, 14, 1, "Game over! Press");
    mvwprintw(side_panel, 15, 1, "start button or");
    mvwprintw(side_panel, 16, 1, "Exit button");
  }
#endif

  mvwprintw(side_panel, 4, 1, "Score: %d", info.score);
  mvwprintw(side_panel, 5, 1, "Level: %d", info.level);
  mvwprintw(side_panel, 6, 1, "High Score: %d", info.high_score);
#ifdef TETRIS_CLI
  mvwprintw(side_panel, 7, 1, "Next Block:");
  // Render next block
  for (int i = 0; i < info.next_block_size; i++) {
    for (int j = 0; j < info.next_block_size; j++) {
      if (info.next_block[i][j]) {
        mvwprintw(side_panel, 8 + i, 2 + 2 * j, "[]");
      }
    }
  }
#endif

  mvwprintw(side_panel, 20, 1, "Arlindad");
  wrefresh(side_panel);

  delwin(side_panel);
  delwin(game_window);
}

/**
 * @brief Catches a keyboard press and depending on the key pressed.
 * @param[in] is_moving If game status = Moving: True, else false
 * @param[in] time_left Time counter until state change
 * @param[in,out] action action player actions
 */
void updateUserAction(bool is_moving, unsigned long long time_left,
                      UserAction_t *action) {
  // проверка на действия
  if (is_moving) {
    timeout(time_left);
  }
  int ch = getch();
  switch (ch) {
    case 'n':
    case 'N':
      *action = UserStart;
      break;
    case 'p':
    case 'P':
      *action = Pause;
      break;
    case KEY_END:
      *action = Terminate;
      break;
    case KEY_LEFT:
      *action = Left;
      break;
    case KEY_RIGHT:
      *action = Right;
      break;
    case KEY_UP:
      *action = Up;
      break;
    case KEY_DOWN:
      *action = Down;
      break;
    case 'r':
    case 'R':
      *action = Action;
      break;
    case ERR:
      break;
    default:
      *action = Noclick;
      break;
  }
}