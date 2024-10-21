/** @file
 * @brief main func for desktop version
 */

#include "desktop.hpp"

#include <ctype.h>
#include <gtk/gtk.h>
#include <string.h>

using namespace s21;

#define GUI_FIELD_SIZE_X 50
#define GUI_FIELD_SIZE_Y 50

static GtkLabel* labels[GUI_FIELD_SIZE_Y][GUI_FIELD_SIZE_X];
static int gui_state = GUI_GAME_PREPARE_RUN;
static int last_keycode_input = -1;
bool is_valid_key = false;
static int last_keyval_input = -1;
static UserAction_t mapped_key_input = Noclick;

#ifdef SNAKE_DESK
static Snake snake;
static Controller_s controller_s(&snake);

void game_s() {
  switch (snake.state_.status) {
    case Start:
      break;
    case Spawn:
      snake.food_init();
      snake.state_.status = Moving;
      break;
    case Moving:
      snake.state_.time_left = snake.process_timer();
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
      break;
    case ExitGame:
      break;
  }
}
#endif

#ifdef TETRIS_DESK
static Tetris tetris;
static Controller_t controller_t(&tetris);

void game_t() {
  switch (tetris.state_.status) {
    case Start:
      break;
    case Spawn:
      tetris.spawn_block();
      break;
    case Moving:
      tetris.state_.time_left = tetris.process_timer();
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
      break;
    case ExitGame:
      break;
  }
}
#endif

int main(int argc, char** argv) {
  GtkApplication* app;
  int status;

#ifdef SNAKE_DESK
  snake.current_state_init();
#endif

#ifdef TETRIS_DESK
  tetris.tetris_state_init();
#endif

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}

/** @brief Refresh field every t seconds. Called from gtk automaticaly
 * @param data some data
 * @return status Always TRUE
 */
gboolean field_refresh(gpointer data) {
#ifdef SNAKE_DESK
  GameInfo_t Game = snake.state_;
#endif

#ifdef TETRIS_DESK
  GameInfo_t Game = controller_t.updateCurrentState();
  // GameInfo_t Game = tetris.state_;
#endif

  if (Game.status == Start) {
    gui_state = GUI_GAME_PREPARE_RUN;
  }
  switch (gui_state) {
    case GUI_GAME_PREPARE_RUN:
      for (int y = 0; y < GUI_FIELD_SIZE_Y; ++y) {
        for (int x = 0; x < GUI_FIELD_SIZE_X; ++x) {
          gtk_label_set_text(labels[y][x], "");
        }
      }

      for (int y = 0; y < FIELD_H + 2; ++y) {
        for (int x = 0; x < FIELD_W + 2; ++x) {
          gtk_label_set_text(labels[y][x], "#");
        }
      }

      gui_state = GUI_GAME_RUN;
      break;

    case GUI_GAME_RUN: {
      static char buf[2] = {0};

#ifdef SNAKE_DESK
      game_s();
#endif

#ifdef TETRIS_DESK
      game_t();
#endif
      if (Game.field) {
        for (int y = 0; y < FIELD_H; ++y) {
          for (int x = 0; x < FIELD_W; ++x) {
            GtkLabel* label = labels[y + 1][x + 1];
            buf[0] = (char)Game.field[y][x];

            if (isascii(Game.field[y][x])) {
              buf[0] = Game.field[y][x];

            } else {
              buf[0] = '0';
            }

            gtk_label_set_text(label, buf);
          }
        }
      }

      int x = FIELD_W + 2, y = 2;
      char tmp_buf[64] = {0};
      gtk_label_set_text(labels[y][x], "High score: ");
      sprintf(tmp_buf, "%d", Game.high_score);
      gtk_label_set_text(labels[y++][x + 1], tmp_buf);

      gtk_label_set_text(labels[y][x], "Score: ");
      sprintf(tmp_buf, "%d", Game.score);
      gtk_label_set_text(labels[y++][x + 1], tmp_buf);

      gtk_label_set_text(labels[y][x], "Level: ");
      sprintf(tmp_buf, "%d", Game.level);
      gtk_label_set_text(labels[y++][x + 1], tmp_buf);

      gtk_label_set_text(labels[y][x], "Speed: ");
      sprintf(tmp_buf, "%d", Game.speed);
      gtk_label_set_text(labels[y++][x + 1], tmp_buf);

      gtk_label_set_text(labels[y][x], "Pause: ");
      if (Game.pause) {
        sprintf(tmp_buf, "%s", "Yes");
      } else {
        sprintf(tmp_buf, "%s", "No");
      }
      gtk_label_set_text(labels[y++][x + 1], tmp_buf);

#ifdef TETRIS_DESK
      gtk_label_set_text(labels[y][x], "Next Block:");
      // y += 1;
      x += 2;
      if (Game.next_block) {
        for (int y1 = 0; y1 < 4; ++y1) {
          for (int x1 = 0; x1 < 4; ++x1) {
            GtkLabel* label = labels[y + y1][x + x1];
            buf[0] = (char)Game.field[y][x];

            if (Game.next_block_size > y1 && Game.next_block_size > x1 &&
                (Game.status != PostGame || Game.status != ExitGame)) {
              if (Game.next_block[y1][x1]) {
                buf[0] = '#';
              } else {
                buf[0] = ' ';
              }
            } else {
              buf[0] = ' ';
            }
            gtk_label_set_text(label, buf);
          }
        }
      }
#endif
      y += 3;
      x += 3;
#ifdef SNAKE_DESK
      if ((Game.status == PostGame || Game.status == ExitGame) &&
          Game.score >= 196) {
        gtk_label_set_text(labels[y++][x], "Congratulations!");
        gtk_label_set_text(labels[y++][x], "you won.");
      } else if (Game.status == PostGame || Game.status == ExitGame) {
        gtk_label_set_text(labels[y++][x], "Game over! Press");
        gtk_label_set_text(labels[y++][x], "start button or");
        gtk_label_set_text(labels[y++][x], "Exit button");
      }
#endif

#ifdef TETRIS_DESK
      if (Game.status == PostGame || Game.status == ExitGame) {
        gtk_label_set_text(labels[y++][x], "Game over! Press");
        gtk_label_set_text(labels[y++][x], "start button or");
        gtk_label_set_text(labels[y++][x], "Exit button");
      }
#endif

    } break;
  }
#ifdef TETRIS_DESK
  free_matrix(Game.field, FIELD_H);
#endif
  return TRUE;
}

static void on_exit_button_clicked(GtkWidget* widget, gpointer user_data) {
  GApplication* app = G_APPLICATION(user_data);
  g_application_quit(app);
}

/** @brief Function for activate gtk application
 * @param app GtkApplication object
 * @param user_data some data
 */
static void activate(GtkApplication* app, gpointer user_data) {
  GtkWidget* window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Brick Console");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

  GtkWidget* grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), grid);

  for (int y = 0; y < GUI_FIELD_SIZE_Y; ++y) {
    for (int x = 0; x < GUI_FIELD_SIZE_X; ++x) {
      labels[y][x] = GTK_LABEL(gtk_label_new("#"));
      gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(labels[y][x]), x, y, 1, 1);
    }
  }

  GtkEventController* keyPressEventController = gtk_event_controller_key_new();
  g_signal_connect_object(keyPressEventController, "key-pressed",
                          G_CALLBACK(key_pressed), window, G_CONNECT_SWAPPED);
  gtk_widget_add_controller(GTK_WIDGET(window), keyPressEventController);

  g_timeout_add(150, field_refresh, NULL);

  gtk_widget_show(window);

  // if (snake.state_.status_game) {
  //   gtk.window.destroy()
  // }
}

/** @brief Process and map pressed keys
 * @param self GtkEventControllerKey object
 * @param keyval ASCII value of pressed key
 * @param keycode Code of pressed key
 * @param state GdkModifierType object
 * @param user_data some data
 * @return status Always TRUE
 */
gboolean key_pressed(GtkEventControllerKey* self, guint keyval, guint keycode,
                     GdkModifierType state, gpointer user_data) {
  last_keycode_input = keycode;
  last_keyval_input = keyval;

  // g_print("[INFO]: key_pressed: keycode:%u, keyval:%u\n", keycode, keyval);

  switch (keyval) {
    case 'n':
    case 'N':
      mapped_key_input = UserStart;
      is_valid_key = true;
      break;
    case 'p':
    case 'P':
      mapped_key_input = Pause;
      is_valid_key = true;
      break;
    case 'r':
    case 'R':
      mapped_key_input = Action;
      is_valid_key = true;
      break;
    case 65362:
      mapped_key_input = Up;
      is_valid_key = true;
      break;
    case 65363:
      mapped_key_input = Right;
      is_valid_key = true;
      break;
    case 65361:
      mapped_key_input = Left;
      is_valid_key = true;
      break;
    case 65364:
      mapped_key_input = Down;
      is_valid_key = true;
      break;
    case 65367:
      mapped_key_input = Terminate;
      is_valid_key = true;
      break;
  }

#ifdef TETRIS_DESK
  if (is_valid_key) {
    controller_t.userInput(mapped_key_input);
    is_valid_key = false;
  }
#endif

#ifdef SNAKE_DESK
  if (is_valid_key) {
    controller_s.userInput(mapped_key_input);
    is_valid_key = false;
  }
#endif
  return TRUE;
}