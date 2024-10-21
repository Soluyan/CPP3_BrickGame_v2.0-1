/**
 * @file model_s.cpp
 * @brief The file contains declarations of the snake game backend functions and
 * macros.
 */
#include "model_s.hpp"

#include <stdlib.h>

#include <cstdio>

#include "../common/common.hpp"

using namespace s21;

Snake::~Snake() {
  delete[] state_.snake;
  free_matrix(state_.field, FIELD_H);
}

/**
 * @brief Initializes the GameInfo_t structure
 * @param[in,out] info_t GameInfo_t structure
 * @version 1.0
 */
void Snake::current_state_init() {
  srand(current_time_millis());
  if (state_.field == NULL) {
    state_.field = create_matrix(FIELD_H, FIELD_W);
  }
  if (state_.field == NULL) {
    exit_with_error();
  } else {
    clear_matrix(state_.field, FIELD_H, FIELD_W);

    state_.status_game = true;
    state_.status = Start;
    state_.pause = UNPAUSE;
    state_.score = 0;
    state_.speed = 1;
    state_.level = 1;
    state_.start_time = 0;
    state_.time_left = 0;
    state_.action = Noclick;
    state_.length = 4;

    snake_init();

    state_.high_score = import_score();
  }
}

/**
 * @brief Loads score from txt file
 * @version 1.0
 */
int Snake::import_score() {
  int high_score = 0;
  FILE *file = fopen("snake_score.txt", "r");
  if (file == NULL) {
    file = fopen("snake_score.txt", "w");
    fprintf(file, "0");
    fclose(file);
    file = fopen("snake_score.txt", "r");
  }
  if (fscanf(file, "%d", &high_score) != 1) {
    perror("Failed to read");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  fclose(file);
  return high_score;
}

/**
 * @brief Exports the score from the game to a text file.
 * @version 1.0
 */
void Snake::export_score() {
  FILE *fp = fopen("snake_score.txt", "w");
  if (fp != NULL) {
    fprintf(fp, "%d", state_.high_score);
    fclose(fp);
  }
}

/**
 * @brief Food initialization on the field
 * @note If food appears in an area where there is a snake, it changes the
 * location of the appearance.
 * @version 1.0
 */
void Snake::food_init() {
  state_.food.x = (rand() % (FIELD_W));
  state_.food.y = (rand() % (FIELD_H));
  for (int i = 0; i < state_.length;) {
    if (state_.food.x == state_.snake[i].x &&
        state_.food.y == state_.snake[i].y) {
      state_.food.x = (rand() % (FIELD_W - 1)) + 1;
      state_.food.y = (rand() % (FIELD_H - 1)) + 1;
      i = 0;
    } else {
      i++;
    }
  }
  state_.field[state_.food.y][state_.food.x] = 1;
}

/**
 * @brief Snake initialization on the field
 * @note The snake's tail is initialized relative to the direction of the head.
 * @version 1.0
 */
void Snake::snake_init() {
  state_.snake = new snake_part[200];
  if (state_.snake == NULL) {
    exit_with_error();
  } else {
    int tail_ori = M_Up;
    // Init for desktop version
    for (int i = 0; i < 200; i++) {
      state_.snake[i].x = 0;
      state_.snake[i].y = 0;
    }

    for (int i = 0; i < state_.length;) {
      if (i == 0) {
        state_.snake[i].x = (rand() % (FIELD_W - 4)) + 2;
        state_.snake[i].y = (rand() % (FIELD_H - 5)) + 2;
        state_.orientation = M_Up;
        i++;
      } else {
        if (tail_ori == M_Left || tail_ori == M_Right) {
          tail_ori = M_Down;
        } else {
          tail_ori = (snake_orientation)((rand() % 3) + 1);
        }
        if (tail_ori == M_Left && state_.snake[i].x != 0) {
          state_.snake[i].x = state_.snake[i - 1].x - 1;
          state_.snake[i].y = state_.snake[i - 1].y;
          i++;
        } else if (tail_ori == M_Down && state_.snake[i].y != 20) {
          state_.snake[i].x = state_.snake[i - 1].x;
          state_.snake[i].y = state_.snake[i - 1].y + 1;
          i++;
        } else if (tail_ori == M_Right && state_.snake[i].x != 10) {
          state_.snake[i].x = state_.snake[i - 1].x + 1;
          state_.snake[i].y = state_.snake[i - 1].y;
          i++;
        }
      }
    }
    snake_drawing();
  }
}

/**
 * @brief Draws a snake on the playing field.
 * @version 1.0
 */
void Snake::snake_drawing() {
  for (int i = 0; i < state_.length; i++) {
    state_.field[state_.snake[i].y][state_.snake[i].x] = 1;
  }
}

/**
 * @brief Moves the snake parts relative to the previous element
 * @version 1.0
 */
void Snake::tail_shift() {
  for (int i = state_.length - 1; i > 0; i--) {
    state_.snake[i].x = state_.snake[i - 1].x;
    state_.snake[i].y = state_.snake[i - 1].y;
  }
}

/**
 * @brief Moves all elements of the snake relative to the direction of movement
 * @version 1.0
 */
void Snake::snake_move() {
  state_.field[state_.snake[state_.length - 1].y]
              [state_.snake[state_.length - 1].x] = 0;
  tail_shift();
  if (state_.orientation == M_Up) {
    state_.snake[0].y = state_.snake[0].y - 1;
  } else if (state_.orientation == M_Down) {
    state_.snake[0].y = state_.snake[0].y + 1;
  } else if (state_.orientation == M_Left) {
    state_.snake[0].x = state_.snake[0].x - 1;
  } else if (state_.orientation == M_Right) {
    state_.snake[0].x = state_.snake[0].x + 1;
  }
  snake_drawing();
}

/**
 * @brief Checks for a collision between the snake's head and an element on the
 * field
 * @param[in] orientation Direction of movement of the snake
 * @param[in] snake_head Snake head coordinates
 * @param[in] element Element coordinates
 * @return status true:Collision false:No collision
 * @version 1.0
 */
bool Snake::snake_collision(snake_orientation orientation,
                            snake_part snake_head, snake_part element) {
  if ((orientation == M_Up && element.y + 1 == snake_head.y &&
       element.x == snake_head.x) ||
      (orientation == M_Down && element.y - 1 == snake_head.y &&
       element.x == snake_head.x) ||
      (orientation == M_Left && element.x + 1 == snake_head.x &&
       element.y == snake_head.y) ||
      (orientation == M_Right && element.x - 1 == snake_head.x &&
       element.y == snake_head.y)) {
    return true;
  }
  return false;
}

/**
 * @brief Checks for collision between the snake's head and its tail
 * @return status true:Collision false:No collision
 * @version 1.0
 */
bool Snake::tail_collision() {
  for (int i = 1; i < state_.length; i++) {
    if (snake_collision(state_.orientation, state_.snake[0], state_.snake[i])) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Checks for collision between the snake's head and field boundaries
 * @return status true:Collision false:No collision
 * @version 1.0
 */
bool Snake::field_collision() {
  if ((state_.orientation == M_Up && state_.snake[0].y - 1 < 0) ||
      (state_.orientation == M_Down && state_.snake[0].y + 1 >= FIELD_H) ||
      (state_.orientation == M_Left && state_.snake[0].x - 1 < 0) ||
      (state_.orientation == M_Right && state_.snake[0].x + 1 >= FIELD_W)) {
    return true;
  }
  return false;
}

/**
 * @brief Moves the snake relative to the direction of movement and also checks
 * for collisions with the edges of the field and with the tail of the snake.
 * @version 1.0
 */
void Snake::snake_shifting() {
  if (snake_collision(state_.orientation, state_.snake[0], state_.food)) {
    state_.status = Attaching;
  } else if (tail_collision() || field_collision()) {
    state_.action = Noclick;
    state_.status = GameOver;
  } else {
    state_.status = Moving;
    snake_move();
  }
}

/**
 * @brief Increases the size of the snake and also goes into the status Spawn
 * @version 1.0
 */
void Snake::food_consume() {
  state_.length += 1;
  state_.score = state_.length - 4;
  if (state_.score <= 45) {
    state_.level = (state_.score / 5) + 1;
    state_.speed = state_.level;
  }
  if (state_.score > state_.high_score) state_.high_score = state_.score;
  tail_shift();
  state_.snake[0].x = state_.food.x;
  state_.snake[0].y = state_.food.y;
  snake_drawing();
  state_.status = Spawn;
}

/**
 * @brief Calculates the time in the game, lowers the block down if time_left <
 * 0.
 * @return returns the remaining time until the block falls down on the playing
 * field.
 */
unsigned long long Snake::process_timer() {
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
    state_.status = Shifting;
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
 * @brief Stops the program if it cannot allocate memory.
 * @version 1.0
 */
void Snake::exit_with_error() {
  std::perror("Couldn't manage to allocate memory");
  exit(EXIT_FAILURE);
}