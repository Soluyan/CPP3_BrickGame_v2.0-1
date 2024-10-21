/** @file gui.hpp
 * @brief main func for terminal version
 */
#ifndef _S21_GUI_HPP_
#define _S21_GUI_HPP_

#include "../../structures.h"

void render(GameInfo_t info);
void updateUserAction(bool is_moving, unsigned long long time_left,
                      UserAction_t *action);
#endif