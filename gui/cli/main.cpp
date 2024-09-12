#include "main.h"

int main() {
  WINDOW *w = initscr();
  cbreak();
  noecho();
  keypad(w, true);
  curs_set(0);

  while (true) {
    s21::SnakeLogic model{};
    s21::Controller controller{&model};
    s21::SnakeLoop snakeLoop{&controller};
    int menuSelection = snakeLoop.menu(w);
    if (menuSelection == 0) {
      clear();
      nodelay(w, true);
      wclear(w);
      snakeLoop.gameLoop();
    } else if (menuSelection == 1) {
      clear();
      nodelay(w, true);
      wclear(w);
      start_color();
      init_pair(1, COLOR_GREEN, COLOR_RED);
      init_pair(2, COLOR_GREEN, COLOR_GREEN);
      init_pair(3, COLOR_GREEN, COLOR_YELLOW);
      init_pair(4, COLOR_GREEN, COLOR_BLUE);
      init_pair(5, COLOR_GREEN, COLOR_MAGENTA);
      init_pair(6, COLOR_GREEN, COLOR_CYAN);
      init_pair(7, COLOR_GREEN, COLOR_WHITE);
      init_pair(8, COLOR_WHITE, COLOR_WHITE);
      refresh();
      Extra_info extr;
      newgame(w, extr);
    } else if (menuSelection == 2) {
      break;
    }

    wclear(w);
    wrefresh(w);
  }

  endwin();
  return 0;
}
