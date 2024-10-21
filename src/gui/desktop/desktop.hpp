/**
 * @file desktop.hpp
 * @brief main func for desktop version.
 */
#ifndef _S21_DESKTOP_HPP_
#define _S21_DESKTOP_HPP_

#include <gtk/gtk.h>

#include "../../brick_game/common/common.hpp"
#include "../../brick_game/snake/controller_s.hpp"
#include "../../brick_game/snake/model_s.hpp"
#include "../../brick_game/tetris/controller_t.hpp"
#include "../../brick_game/tetris/model_t.hpp"
#include "../../structures.h"

using namespace s21;

enum gui_states {
  GUI_GAME_PREPARE_RUN,
  GUI_GAME_RUN,
};

static void activate(GtkApplication* app, gpointer user_data);

gboolean field_refresh(gpointer data);

gboolean key_pressed(GtkEventControllerKey* self, guint keyval, guint keycode,
                     GdkModifierType state, gpointer user_data);

#endif