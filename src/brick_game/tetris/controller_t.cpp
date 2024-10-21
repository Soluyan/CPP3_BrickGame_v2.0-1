/** @file controller_t.cpp
 * @brief backend for tetris control functions.
 */
#include "controller_t.hpp"

#include <stdio.h>

#include "../../structures.h"
#include "../common/common.hpp"

using namespace s21;

/**
 * @brief Накладывает Поверх игрового поля текущий блок тетриса.
 * @return Возвращает для GUI Структуру GameInfo_t;
 */
GameInfo_t Controller_t::updateCurrentState() {
  GameInfo_t Game_state = tetris->state_;

  int **render_field = create_matrix(FIELD_H, FIELD_W);
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      render_field[i][j] = tetris->state_.field[i][j];
    }
  }

  for (int i = 0; i < tetris->state_.block_size; i++) {
    for (int j = 0; j < tetris->state_.block_size; j++) {
      if (tetris->state_.block[i][j]) {
        // возможно нужно будет добавить отлов выхода из массива
        render_field[tetris->state_.coord_x + i][tetris->state_.coord_y + j] =
            tetris->state_.block[i][j];
      }
    }
  }

  Game_state.field = render_field;

  return Game_state;
}

/**
 * @brief В зависимости от action изменяет параметры игры.
 * @param action Состояние нажатия кнопки игроком.
 */
void Controller_t::userInput(UserAction_t action) {
  if (action == UserStart &&
      (tetris->state_.status == Start || tetris->state_.status == PostGame)) {
    if (tetris->state_.status == Start)
      tetris->state_.status = Spawn;
    else {
      tetris->tetris_state_init();
      tetris->state_.status = Start;
    }
    tetris->state_.action = Noclick;
  } else if (action == Pause && tetris->state_.status != Start &&
             tetris->state_.status != GameOver) {
    pause_game();
    tetris->state_.action = Noclick;
  } else if (action == Terminate && tetris->state_.status != Start) {
    if (tetris->state_.status == PostGame) {
      terminate_game();
      tetris->state_.action = Noclick;
    } else {
      game_over();
      tetris->state_.action = Noclick;
    }
  } else if (action == Left && tetris->state_.status == Moving &&
             tetris->state_.pause == UNPAUSE) {
    if (tetris->can_move_left()) {
      tetris->state_.coord_y -= 1;
    }
    tetris->state_.action = Noclick;
  } else if (action == Right && tetris->state_.status == Moving &&
             tetris->state_.pause == UNPAUSE) {
    if (tetris->can_move_right()) {
      tetris->state_.coord_y += 1;
    }
    tetris->state_.action = Noclick;
  } else if (action == Down && tetris->state_.status == Moving &&
             tetris->state_.pause == UNPAUSE) {
    tetris->shifting_block();
    tetris->state_.action = Noclick;
  } else if (action == Action && tetris->state_.status == Moving &&
             tetris->state_.pause == UNPAUSE) {
    if (tetris->can_rotate_block()) tetris->rotate_block();
    tetris->state_.action = Noclick;
  }
}

/**
 * @brief Замораживает игру;
 */
void Controller_t::pause_game() {
  tetris->state_.pause = !tetris->state_.pause;
}

/**
 * @brief Если игра окончена ставит игру на паузу и сохраняет score в
 * текстовый документ, также переводит игру в состояние PostGame;
 */
void Controller_t::game_over() {
  tetris->state_.status = PostGame;
  // Game->action = Noclick;
  pause_game();
  FILE *fp = fopen("tetris_score.txt", "w");
  if (fp != NULL) {
    fprintf(fp, "%d", tetris->state_.high_score);
    fclose(fp);
  }
}

/**
 * @brief Очищает всю динамически занятую память и переводит в состояние
 * выхода из игры(ExitGame);
 */
void Controller_t::terminate_game() {
  tetris->state_.status = ExitGame;
  tetris->state_.status_game = false;
}

void Controller_t::cleanup_game() {
  free_matrix(tetris->state_.field, FIELD_H);
}