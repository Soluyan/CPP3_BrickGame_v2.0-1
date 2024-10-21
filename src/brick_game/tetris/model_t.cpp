/**
 * @file model_t.cpp
 * @brief The file contains declarations of the tetris game backend functions
 * and macros.
 */
#include "model_t.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "../common/common.hpp"

using namespace s21;

void Tetris::set_current_state_null() {
  state_.field = NULL;
  state_.block = NULL;
  state_.next_block = NULL;
  state_.score = 0;
  state_.high_score = 0;
  state_.level = 0;
  state_.speed = 0;
  state_.pause = UNPAUSE;
  state_.status = Start;
  state_.block_size = 0;
  state_.next_block_size = 0;
  state_.coord_x = 0;
  state_.coord_y = 0;
  state_.start_time = 0;
  state_.time_left = 0;
}

/**
 * @brief Устанаввливает первичные параметры при запуске новой игры, а также
 * загружает текстовый документ с рекордом в игре
 */
void Tetris::tetris_state_init() {
  state_.status = Start;
  state_.status_game = true;

  int **field = create_matrix(FIELD_H, FIELD_W);
  int next_temp_block_size = 0;
  int **temp_next_block = generate_new_block(&next_temp_block_size);
  state_.field = field;
  state_.score = 0;
  state_.level = 0;
  state_.speed = 1;
  state_.time_left = RESET_TIME;
  state_.pause = UNPAUSE;
  state_.coord_x = -1;
  state_.coord_y = 4;
  state_.block = NULL;
  state_.next_block = temp_next_block;
  state_.block_size = 0;
  state_.next_block_size = next_temp_block_size;

  int high_score = 0;
  FILE *file = fopen("tetris_score.txt", "r");
  if (file == NULL) {
    file = fopen("tetris_score.txt", "w");
    fprintf(file, "0");
    fclose(file);
    file = fopen("tetris_score.txt", "r");
  }
  if (fscanf(file, "%d", &high_score) != 1) {
    perror("Failed to read");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  fclose(file);
  state_.high_score = high_score;  // подгрузка из txt файла

  srand(current_time_millis());
}

/**
 * @brief Генерируе случайную игровую фигуру.
 * @param block_size Параметр сохраняющий размерновой фигуры.
 * @return Сгенерированную матрицу содержащую фигуру для тетриса;
 */
int **Tetris::generate_new_block(int *block_size) {
  int **block;
  int block_type = rand() % 7;
  if (block_type == 0) {
    *block_size = 4;
    block = create_matrix(*block_size, *block_size);
  } else if (block_type == 1) {
    *block_size = 2;
    block = create_matrix(*block_size, *block_size);
  } else {
    *block_size = 3;
    block = create_matrix(*block_size, *block_size);
  }

  if (block_type == 0) {
    block[1][0] = 1;
    block[1][1] = 1;
    block[1][2] = 1;  // I
    block[1][3] = 1;
  } else if (block_type == 1) {
    block[0][0] = 1;
    block[0][1] = 1;
    block[1][0] = 1;  // O
    block[1][1] = 1;
  } else if (block_type == 2) {
    block[0][0] = 1;
    block[0][1] = 1;
    block[0][2] = 1;  // T
    block[1][1] = 1;
  } else if (block_type == 3) {
    block[0][0] = 1;
    block[1][0] = 1;
    block[1][1] = 1;  // Г
    block[1][2] = 1;
  } else if (block_type == 4) {
    block[0][2] = 1;
    block[1][0] = 1;
    block[1][1] = 1;  // Г(<-)
    block[1][2] = 1;
  } else if (block_type == 5) {
    block[0][0] = 1;
    block[0][1] = 1;
    block[1][1] = 1;  // Z
    block[1][2] = 1;
  } else if (block_type == 6) {
    block[1][0] = 1;
    block[1][1] = 1;
    block[0][1] = 1;  // Z(<-)
    block[0][2] = 1;
  }
  return block;
}

/**
 * @brief Переводит параметры координат для новой фигуры в страндартные
 * значения, очищая предыдущую матрицу с фигурой заменяя ее новой созданной
 * фигурой из next_block. Переводит игру в состояние падения фигуры на игровом
 * поле
 */
void Tetris::spawn_block() {
  if (state_.block != NULL) {
    free_matrix(state_.block, state_.block_size);
    // Game->block = NULL;
  }
  state_.block = state_.next_block;
  state_.block_size = state_.next_block_size;
  state_.coord_x = 0;
  state_.coord_y = (FIELD_W - state_.block_size) / 2;
  state_.next_block = generate_new_block(&state_.next_block_size);
  state_.status = Moving;
}

/**
 * @brief Разворачивает фигуру на 90 градусов вправо.
 * @param block Матрица с фигурурой.
 * @param block_size Параметр размера квадратной матрицы фигуры.
 */
void Tetris::rotate_block() {
  int **temp_block = create_matrix(state_.block_size, state_.block_size);

  if (state_.block_size == 4) {
    if (state_.block[1][0] == 1) {
      temp_block[0][1] = 1;
      temp_block[2][1] = 1;
      temp_block[3][1] = 1;
      temp_block[1][1] = 1;
    } else {
      temp_block[1][1] = 1;
      temp_block[1][0] = 1;
      temp_block[1][2] = 1;
      temp_block[1][3] = 1;
    }
  } else if (state_.block_size == 3) {
    for (int i = 0; i < state_.block_size; i++) {
      for (int j = 0; j < state_.block_size; j++) {
        temp_block[j][state_.block_size - 1 - i] = state_.block[i][j];
      }
    }
  }
  if (state_.block_size == 2) {
    free_matrix(temp_block, state_.block_size);
  } else {
    free_matrix(state_.block, state_.block_size);
    state_.block = temp_block;
  }
}

/**
 * @brief Проверяет возможности разворота фигуры
 * @return True - разворота фигуры допустим; \
 * @return False - разворота фигуры недопустим; \
 */
bool Tetris::can_rotate_block() {
  int temp_block[state_.block_size][state_.block_size];
  bool can_rotate = true;

  // Rotate
  for (int i = 0; i < state_.block_size; i++) {
    for (int j = 0; j < state_.block_size; j++) {
      temp_block[j][state_.block_size - 1 - i] = state_.block[i][j];
    }
  }

  // Check rotated block
  for (int i = 0; i < state_.block_size; i++) {
    for (int j = 0; j < state_.block_size; j++) {
      int new_x = state_.coord_x - i + 1;
      int new_y = state_.coord_y + j;
      if ((temp_block[i][j] == 1) && ((new_y < 0) || (new_y >= FIELD_W))) {
        can_rotate = false;
      } else if ((new_y >= 0) && (new_y < FIELD_W) && (new_x >= 0)) {
        if (temp_block[i][j] && state_.field[new_x][new_y]) {
          can_rotate = false;
        } else if (temp_block[i][j] && state_.field[new_x + 1][new_y]) {
          can_rotate = false;
        }
      }
    }
  }

  return can_rotate;
}

/**
 * @brief Проверяет возможности сдвига фигуры влево
 * @return True - сдвиг фигуры допустим; \
 * @return False - сдвиг фигуры недопустим; \
 */
bool Tetris::can_move_left() {
  for (int i = 0; i < state_.block_size; i++) {
    for (int j = 0; j < state_.block_size; j++) {
      if (state_.block[i][j] &&
          (state_.coord_y + j - 1 < 0 ||
           state_.field[state_.coord_x + i][state_.coord_y + j - 1])) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Проверяет возможности сдвига фигуры вправо
 * @return True - сдвиг фигуры допустим; \
 * @return False - сдвиг фигуры недопустим; \
 */
bool Tetris::can_move_right() {
  for (int i = 0; i < state_.block_size; i++) {
    for (int j = 0; j < state_.block_size; j++) {
      if (state_.block[i][j] &&
          (state_.coord_y + j + 1 >= FIELD_W ||
           state_.field[state_.coord_x + i][state_.coord_y + j + 1])) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Проверяет возможности сдвига фигуры вниз
 * @return True - сдвиг фигуры допустим; \
 * @return False - сдвиг фигуры недопустим; \
 */
bool Tetris::can_move_down() {
  for (int i = 0; i < state_.block_size; i++) {
    for (int j = 0; j < state_.block_size; j++) {
      if (state_.block[i][j] &&
          (state_.coord_x + i + 1 >= FIELD_H ||
           state_.field[state_.coord_x + i + 1][state_.coord_y + j])) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Если это возможно сдвигает фигуру вниз по координате X иначе
 * присоединят фигуру к игровому полю;
 */
void Tetris::shifting_block() {
  if (can_move_down()) {
    state_.coord_x += 1;
  } else {
    attaching_block();
  }
}

/**
 * @brief Очищает полностью заполненные линии в поле для игры, начисляет
 * дополнительные очки за множественное очищение линий за один раз;
 */
void Tetris::clear_full_lines() {
  int combo_line = 1;
  for (int i = 0; i < FIELD_H; i++) {
    int full_line = 1;
    for (int j = 0; j < FIELD_W; j++) {
      if (!state_.field[i][j]) {
        full_line = 0;
        break;
      }
    }
    if (full_line) {
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < FIELD_W; j++) {
          state_.field[k][j] = state_.field[k - 1][j];
        }
      }
      for (int j = 0; j < FIELD_W; j++) {
        state_.field[0][j] = 0;
      }
      // сделать дополнительное начисление очков за множествое очешение линий
      state_.score += 100;
      if (combo_line == 2) {
        state_.score += 100;
      } else if (combo_line == 3) {
        state_.score += 200;
      } else if (combo_line == 4) {
        state_.score += 800;
      }
      combo_line++;
    }
    if (state_.score > state_.high_score) state_.high_score = state_.score;
  }
}

/**
 * @brief Накладывет на field матрицу с фигурой block;
 */
void Tetris::attach_block_to_field() {
  for (int i = 0; i < state_.block_size; i++) {
    for (int j = 0; j < state_.block_size; j++) {
      if (state_.block[i][j]) {
        state_.field[state_.coord_x + i][state_.coord_y + j] = 1;
      }
    }
  }
}

/**
 * @brief Присоединяет фигуру к игровому полю, а также очищает полностью
 * заполненные линии field, если игра не окончена переводит игру в состояние
 * Spawn, иначе в GameOver;
 */
void Tetris::attaching_block() {
  attach_block_to_field();
  clear_full_lines();
  state_.level = (int)(state_.score / 600);
  if (state_.level > 10) state_.level = 10;
  state_.speed = 1 + state_.level;
  if (is_game_over()) {
    state_.status = GameOver;
  } else {
    state_.status = Spawn;
  }
}

/**
 * @brief Подсчитывает время в игре, опускает block вниз если time_left < 0
 * @return возвращает оставшееся время до падения блока вниз на игровом поле;
 */
unsigned long long Tetris::process_timer() {
  unsigned long long current_time = current_time_millis();

  if (state_.pause) {
    return state_.time_left;
  }

  if (state_.start_time == 0) {
    state_.start_time = current_time;
  }

  unsigned long long elapsed_time = current_time - state_.start_time;
  unsigned long long time_left =
      state_.time_left > elapsed_time ? state_.time_left - elapsed_time : 0;

  if (time_left <= 0) {
    shifting_block();
    state_.start_time = current_time;
    if (state_.speed == 1) {
      state_.time_left = RESET_TIME;
    } else {
      state_.time_left = RESET_TIME / (state_.speed / 1.5);
    }
    time_left = state_.time_left;
  }

  return time_left;
}

/**
 * @brief Проверяет возможности окончание игры
 * @return True - Игра окончена; \
 * @return False -  Игра неокончена; \
 */
bool Tetris::is_game_over() {
  for (int j = 0; j < FIELD_W; j++) {
    if (state_.field[0][j] != 0) {
      return true;
    }
  }
  return false;
}