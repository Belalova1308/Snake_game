#ifndef MODEL_H
#define MODEL_H

#include <time.h>
#include <unistd.h>

#include <cstdlib>
#include <list>
#include <random>

#include "../../common/common.h"

namespace s21 {
enum square { empty = 0, food = 1, wall = 2, snake = 3 };
typedef enum { StartGame, Spawn, Moving, Shifting, End } SnakeState_t;
typedef enum { UpDir, DownDir, RightDir, LeftDir } Dir;
class SnakeLogic {
 public:
  struct timespec ts;
  SnakeState_t state;
  Dir direction;
  SnakeLogic();
  ~SnakeLogic();
  bool start = false;
  GameInfo_t info;
  void createSnake();
  void createApple();
  void clearField();
  void moveSnake();
  int checkPos();
  void intoField();
  void setSnakeScore(int high_score);
  int getSnakeScore();
  GameInfo_t speedUp(GameInfo_t &info);
  std::list<std::pair<int, int>> _snake;
  std::pair<int, int> _apple;
};

}  // namespace s21

#endif