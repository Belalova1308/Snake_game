#ifndef SNAKE_LOOP_H
#define SNAKE_LOOP_H
#include <ncurses.h>

#include <string>

#include "../../brick_game/snake/controller/controller.h"

namespace s21 {
enum colors { snake_color = 1, food_color = 2, wall_color = 3 };
class SnakeLoop {
 public:
  Controller *controller;
  SnakeLoop(Controller *c);
  ~SnakeLoop();
  int menu(WINDOW *w);
  void pauseMenu(GameInfo_t *gameInfo);
  void gameLoop();
  UserAction_t getAction(int user_input);
  void drawField(GameInfo_t gameInfo);
  void showInfo(GameInfo_t gameInfo);
};
}  // namespace s21

#endif