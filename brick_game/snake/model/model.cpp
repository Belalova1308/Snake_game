#include "model.h"

namespace s21 {
SnakeLogic::SnakeLogic() {
  info.field = new int *[22];
  for (int y = 0; y < 22; ++y) {
    info.field[y] = new int[12]();
  }
  _snake.clear();
  state = StartGame;
  direction = UpDir;
  info.score = info.level = 0;
  info.pause = false;
  info.speed = 500;
  info.level = 0;
  info.high_score = getSnakeScore();
  clock_gettime(CLOCK_REALTIME, &ts);
}
SnakeLogic::~SnakeLogic() {
  for (int i = 0; i < 22; ++i) {
    delete[] info.field[i];
  }
  delete[] info.field;
}
void SnakeLogic::clearField(void) {
  for (int y = 1; y < 21; ++y) {
    for (int x = 1; x < 11; ++x) {
      info.field[y][x] = 0;
    }
  }
}
void SnakeLogic::createApple() {
  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<int> distributionY(1, 19);
  std::uniform_int_distribution<int> distributionX(1, 9);
  _apple = std::make_pair(distributionY(generator), distributionX(generator));
}

void SnakeLogic::createSnake() {
  _snake.clear();
  _snake.push_back(std::make_pair(20 / 2, 10 / 2));
  _snake.push_back(std::make_pair(20 / 2 + 1, 10 / 2));
  _snake.push_back(std::make_pair(20 / 2 + 2, 10 / 2));
  _snake.push_back(std::make_pair(20 / 2 + 3, 10 / 2));
}
void SnakeLogic::intoField() {
  if (_apple.first >= 0 && _apple.first < 22 && _apple.second >= 0 &&
      _apple.second < 12) {
    info.field[_apple.first][_apple.second] = food;
  }

  for (const auto &pos : _snake) {
    int y = pos.first;
    int x = pos.second;
    if (y >= 0 && y < 22 && x >= 0 && x < 12) {
      info.field[y][x] = snake;
    }
  }
  for (int y = 0; y < 22; y++) {
    for (int x = 0; x < 12; x++) {
      if (x == 0 || x == 11 || y == 0 || y == 21) info.field[y][x] = wall;
    }
  }
}
int SnakeLogic::getSnakeScore() {
  int high_score = 0;
  FILE *f = fopen("./score.txt", "r");
  if (f != NULL) {
    int res = fscanf(f, "%d", &high_score);
    if (res) fclose(f);
  }
  return high_score;
}

void SnakeLogic::setSnakeScore(int high_score) {
  FILE *f = fopen("./score.txt", "w");
  if (f != NULL) {
    fprintf(f, "%d", high_score);
    fclose(f);
  }
}
int SnakeLogic::checkPos() {
  auto head = _snake.front();
  if (head == _apple) {
    info.score++;
    speedUp(info);
    return food;
  } else if (head.first < 1 || head.second < 1 || head.first > 20 ||
             head.second > 10) {
    return wall;
  }
  auto iter = _snake.begin();
  iter++;
  iter++;
  iter++;
  for (; iter != _snake.end(); ++iter) {
    if (*iter == head) {
      return snake;
    }
  }
  return empty;
}
void SnakeLogic::moveSnake() {
  auto p = _snake.front();
  switch (direction) {
    case UpDir:
      p.first = p.first - 1;
      break;
    case DownDir:
      p.first = p.first + 1;
      break;
    case LeftDir:
      p.second = p.second - 1;
      break;
    case RightDir:
      p.second = p.second + 1;
      break;
    default:
      break;
  }
  _snake.push_front(p);
  int check = checkPos();
  if (info.score > info.high_score) {
    info.high_score = info.score;
    setSnakeScore(info.score);
  }
  if (check == food) {
    state = Spawn;
  } else if (check == wall || check == snake) {
    state = End;
  } else if (check == empty) {
    state = Moving;
    _snake.pop_back();
  }
  if (info.level == 10 || _snake.size() == 200) {
    state = End;
  }
  clock_gettime(CLOCK_REALTIME, &ts);
  clearField();
  intoField();
}
GameInfo_t SnakeLogic::speedUp(GameInfo_t &info) {
  int prevLevel = info.level;
  info.level = info.score / 5;
  if (prevLevel < info.level) {
    info.speed -= 20;
  }
  return info;
}
}  // namespace s21