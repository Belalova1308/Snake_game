#include "controller.h"

namespace s21 {

Controller::Controller(SnakeLogic *model) : model{model} {};
Controller::~Controller(){};

void Controller::userInput(UserAction_t action, bool hold) {
  Dir temp = model->direction;
  switch (action) {
    case Start:
      model->start = true;
      break;
    case Left:
      if (temp != RightDir) model->direction = LeftDir;
      break;
    case Right:
      if (temp != LeftDir) model->direction = RightDir;
      break;
    case Up:
      if (temp != DownDir) model->direction = UpDir;
      break;
    case Down:
      if (temp != UpDir) model->direction = DownDir;
      break;
    case Action:
      if (hold) model->moveSnake();
      break;
    case Pause:
      model->info.pause = true;
      break;
    case Terminate:
      model->state = End;
      break;
    default:
      break;
  }
}

GameInfo_t Controller::updateCurrentState() {
  struct timespec current;
  double difference;
  switch (model->state) {
    case StartGame:
      model->createApple();
      model->createSnake();
      model->state = Moving;
      break;
    case Spawn:
      model->createApple();
      model->state = Moving;
      break;
    case Moving:
      clock_gettime(CLOCK_REALTIME, &current);
      difference = (current.tv_sec - model->ts.tv_sec) * 1000 +
                   (current.tv_nsec - model->ts.tv_nsec) / 1000000;
      if (difference >= model->info.speed) {
        model->state = Shifting;
      }
      break;
    case Shifting:
      model->moveSnake();
      break;
    case End:
      model->clearField();
      model->_snake.clear();
      break;
    default:
      break;
  }
  return model->info;
}
}  // namespace s21