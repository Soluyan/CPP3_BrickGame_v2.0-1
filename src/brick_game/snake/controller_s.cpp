/** @file controller_s.cpp
 * @brief backend for snake control functions.
 */
#include "controller_s.hpp"

using namespace s21;

/**
 * @brief Overlays the current game state on top of the playing field.
 * @return Returns the GameInfo_t Structure for the GUI;
 */
GameInfo_t Controller_s::updateCurrentState() {
  GameInfo_t stats = snake->state_;
  return stats;
}

/**
 * @brief Changes the game state relative to the player's action.
 * @param[in] action player actions
 */
void Controller_s::userInput(UserAction_t action) {
  if (action == UserStart &&
      (snake->state_.status == Start || snake->state_.status == PostGame)) {
    if (snake->state_.status == Start)
      snake->state_.status = Spawn;
    else {
      delete[] snake->state_.snake;
      snake->current_state_init();
    }
  } else if (action == Pause && snake->state_.status != Start &&
             snake->state_.status != GameOver &&
             snake->state_.status != PostGame) {
    pause_game();
    snake->state_.action = Noclick;
  } else if (action == Terminate && snake->state_.status != Start) {
    snake->state_.status == PostGame ? terminate_game() : game_over();
    snake->state_.action = Noclick;
  } else if (action == Left && snake->state_.status == Moving &&
             snake->state_.pause == UNPAUSE &&
             snake->state_.orientation != M_Right) {
    snake->state_.orientation = M_Left;
  } else if (action == Right && snake->state_.status == Moving &&
             snake->state_.pause == UNPAUSE &&
             snake->state_.orientation != M_Left) {
    snake->state_.orientation = M_Right;
  } else if (action == Up && snake->state_.status == Moving &&
             snake->state_.pause == UNPAUSE &&
             snake->state_.orientation != M_Down) {
    snake->state_.orientation = M_Up;
  } else if (action == Down && snake->state_.status == Moving &&
             snake->state_.pause == UNPAUSE &&
             snake->state_.orientation != M_Up) {
    snake->state_.orientation = M_Down;
  } else if (action == Action && snake->state_.status == Moving &&
             snake->state_.pause == UNPAUSE) {
    snake->state_.status = Shifting;
  }
}

/**
 * @brief Changes the game state to paused.
 * @note Freezes the game;
 */
void Controller_s::pause_game() { snake->state_.pause = !snake->state_.pause; }

/**
 * @brief If the game is over pauses the game and saves the score to a text
 * document, also puts the game into the PostGame state;
 */
void Controller_s::game_over() {
  snake->state_.status = PostGame;
  snake->state_.pause = PAUSE;
  snake->export_score();
}

/**
 * @brief Clears all dynamically occupied memory and switches to the ExitGame
 * state.
 */
void Controller_s::terminate_game() {
  snake->state_.status = ExitGame;
  snake->state_.status_game = false;
}