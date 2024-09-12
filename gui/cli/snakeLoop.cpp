#include "snakeLoop.h"

namespace s21 {

SnakeLoop::SnakeLoop(Controller *c) : controller{c} {};
SnakeLoop::~SnakeLoop(){};

void SnakeLoop::gameLoop() {
  start_color();
  init_pair(snake_color, COLOR_YELLOW, COLOR_GREEN);
  init_pair(food_color, COLOR_GREEN, COLOR_RED);
  init_pair(wall_color, COLOR_WHITE, COLOR_WHITE);
  refresh();
  while (controller->model->state != End) {
    int c = getch();
    bool hold = false;
    UserAction_t actionInput = getAction(c);
    if (actionInput == Action) hold = true;
    controller->userInput(actionInput, hold);
    GameInfo_t display = controller->updateCurrentState();
    if (actionInput == Pause) pauseMenu(&display);
    drawField(display);
    showInfo(display);
  }
  endwin();
}
UserAction_t SnakeLoop::getAction(int user_input) {
  UserAction_t rc;
  if (user_input == KEY_UP)
    rc = Up;
  else if (user_input == KEY_DOWN)
    rc = Down;
  else if (user_input == KEY_LEFT)
    rc = Left;
  else if (user_input == KEY_RIGHT)
    rc = Right;
  else if (user_input == 27)
    rc = Terminate;
  else if (user_input == '\n')
    rc = Start;
  else if (user_input == 'p')
    rc = Pause;
  else if (user_input == 'e')
    rc = Action;
  return rc;
}
void SnakeLoop::drawField(GameInfo_t gameInfo) {
  for (int y = 0; y < 22; y++) {
    for (int x = 0; x < 12; x++) {
      int cellValue = gameInfo.field[y][x];
      switch (cellValue) {
        case empty:
          mvprintw(y, x, " ");
          break;
        case wall:
          attron(COLOR_PAIR(wall_color));
          mvprintw(y, x, " ");
          attroff(COLOR_PAIR(wall_color));
          break;
        case food:
          attron(COLOR_PAIR(food_color));
          mvprintw(y, x, " ");
          attroff(COLOR_PAIR(food_color));
          break;
        case snake:
          attron(COLOR_PAIR(snake_color));
          mvprintw(y, x, " ");
          attroff(COLOR_PAIR(snake_color));
          break;
        default:
          break;
      }
    }
  }
  refresh();
}
void SnakeLoop::showInfo(GameInfo_t gameInfo) {
  mvprintw(1, 14, "SCORE: %d", gameInfo.score);
  mvprintw(2, 14, "LEVEL: %d", gameInfo.level);
  mvprintw(3, 14, "HIGH SCORE %d", gameInfo.high_score);
  refresh();
}
int SnakeLoop::menu(WINDOW *w) {
  std::string menu[3] = {"Snake", "Tetris", " Quit"};
  int current = 0;

  nodelay(w, false);
  wclear(w);

  while (1) {
    for (int i = 0; i < 3; i++)
      if (i == current) {
        mvprintw(2 + i, 2, ">> %s <<", menu[i].c_str());
      } else {
        mvprintw(2 + i, 2, "   %s   ", menu[i].c_str());
      }

    UserAction_t action = getAction(getch());

    switch (action) {
      case Up:
        current = current == 0 ? 1 : current - 1;
        break;
      case Down:
        current = current == 2 ? 0 : current + 1;
        break;
      case Start:

        return current;
        break;
      default:
        break;
    }
  }
}
void SnakeLoop::pauseMenu(GameInfo_t *gameInfo) {
  WINDOW *pausew = newwin(20, 10, 1, 1);
  box(pausew, ' ', ' ');
  mvwprintw(pausew, 1, 2, "STOPED");
  mvwprintw(pausew, 3, 1, "press 'p'");
  mvwprintw(pausew, 5, 1, "to PLAY");
  wrefresh(pausew);
  int c = getch();
  while (c != 'p') c = getch();
  delwin(pausew);
  gameInfo->pause = false;
}

}  // namespace s21