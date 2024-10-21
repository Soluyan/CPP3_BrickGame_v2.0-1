#ifndef _S21_STRUCTURE_HPP_
#define _S21_STRUCTURE_HPP_

#define RESET_TIME 700
#define FIELD_W 10
#define FIELD_H 20
#define UNPAUSE 0
#define PAUSE 1

// Snake structures
typedef enum { M_Up, M_Left, M_Down, M_Right } snake_orientation;

typedef struct snake_part {
  int x;
  int y;
} snake_part;

typedef enum {
  Start,
  Spawn,
  Moving,
  Shifting,
  Attaching,
  GameOver,
  PostGame,
  ExitGame,
} GameStatus_t;

// Tetris structures
typedef enum { None, GameStart, GamePause } Banner;

// Generic structures
typedef enum {
  UserStart,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Noclick
} UserAction_t;

typedef struct {
  // Generic variables
  int **field;
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
  unsigned long long start_time;
  unsigned long long time_left;
  bool status_game;
  // Tetris status
  int **block;
  int **next_block;
  int block_size;
  int next_block_size;
  int coord_x;
  int coord_y;
  // Snake status
  int length;
  snake_orientation orientation;
  GameStatus_t status;
  UserAction_t action;
  snake_part *snake;
  snake_part food;
} GameInfo_t;

#endif