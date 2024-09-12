#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../../common/common.h"
#include "../model/model.h"

namespace s21 {
class Controller {
 public:
  SnakeLogic *model;
  Controller(SnakeLogic *model);
  ~Controller();
  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();
};

}  // namespace s21

#endif