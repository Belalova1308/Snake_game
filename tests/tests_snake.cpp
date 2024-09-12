#include "tests_main.h"

TEST(state_test, test_1) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
  controller.userInput(Pause, false);
  EXPECT_EQ(model.state, s21::Moving);
  controller.updateCurrentState();
}

TEST(state_test, test_2) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Left, false);
  for (int i{}; i < 9; ++i) model.moveSnake();
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::End);
}

TEST(state_test, test_3) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Left, false);
  model.moveSnake();
  controller.userInput(Up, false);
  model.moveSnake();
  controller.userInput(Right, false);
  model.moveSnake();
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(state_test, test_4) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Action, true);
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(state_test, test_5) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Terminate, false);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::End);
}

TEST(state_test, test_6) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  controller.userInput(Down, false);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(state_test, test_7) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  model._apple.second = 5;
  model._apple.first = 9;
  model.moveSnake();
  model.state = s21::Spawn;
  EXPECT_EQ(model.state, s21::Spawn);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(state_test, test_8) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  controller.userInput(Start, false);
  controller.updateCurrentState();
  usleep(1000000);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Shifting);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(info_test, test_1) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  EXPECT_EQ(model.info.level, 0);
  controller.userInput(Start, false);
  model.info.score = 4;
  model.createSnake();
  EXPECT_EQ(model._snake.front().first, 10);
  EXPECT_EQ(model._snake.front().second, 5);
  model.info.score = 4;
  model._apple.second = 5;
  model._apple.first = 9;
  model.moveSnake();
  EXPECT_EQ(model.state, s21::Spawn);
  controller.updateCurrentState();
  EXPECT_EQ(model.info.level, 1);
  EXPECT_EQ(model.info.score, 5);
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(info_test, test_2) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  EXPECT_EQ(model.info.level, 0);
  controller.userInput(Start, false);
  model.createSnake();
  model.info.level = 10;
  model.info.score = 54;
  model._apple.second = 5;
  model._apple.first = 9;
  model.moveSnake();
  EXPECT_EQ(model.info.level, 11);
  EXPECT_EQ(model.info.score, 55);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(info_test, test_3) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  EXPECT_EQ(model.info.level, 0);
  controller.userInput(Start, false);
  model.createSnake();
  model._apple.second = 5;
  model._apple.first = 9;
  model.moveSnake();
  EXPECT_EQ(model.info.level, 0);
  EXPECT_EQ(model.info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(info_test, test_4) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  EXPECT_EQ(model.info.level, 0);
  controller.userInput(Start, false);
  controller.userInput(Left, false);
  model.createSnake();
  model._apple.second = 4;
  model._apple.first = 10;
  model.moveSnake();
  EXPECT_EQ(model.info.level, 0);
  EXPECT_EQ(model.info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(info_test, test_5) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  EXPECT_EQ(model.info.level, 0);
  controller.userInput(Start, false);
  controller.userInput(Right, false);
  model.createSnake();
  model._apple.second = 6;
  model._apple.first = 10;
  model.moveSnake();
  EXPECT_EQ(model.info.level, 0);
  EXPECT_EQ(model.info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}

TEST(info_test, test_6) {
  s21::SnakeLogic model{};
  s21::Controller controller{&model};
  EXPECT_EQ(model.state, s21::StartGame);
  EXPECT_EQ(model.info.level, 0);
  controller.userInput(Start, false);
  model.createSnake();
  model._apple.second = 6;
  model._apple.first = 9;
  controller.userInput(Right, false);
  model.moveSnake();
  controller.userInput(Up, false);
  model.moveSnake();
  EXPECT_EQ(model.info.level, 0);
  EXPECT_EQ(model.info.score, 1);
  controller.updateCurrentState();
  EXPECT_EQ(model.state, s21::Moving);
}
