#include "snakeImgui.h"

SnakeApp::SnakeApp(s21::Controller *c) : controller{c} {};
SnakeApp::~SnakeApp(){};

UserAction_t SnakeApp::getAction() {
  UserAction_t rc;  // Default action

  if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
    rc = Up;
  else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
    rc = Down;
  else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
    rc = Left;
  else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
    rc = Right;
  else if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    rc = Terminate;
  else if (ImGui::IsKeyPressed(ImGuiKey_Enter))
    rc = Start;
  else if (ImGui::IsKeyPressed(ImGuiKey_P))
    rc = Pause;
  else if (ImGui::IsKeyPressed(ImGuiKey_E))
    rc = Action;
  return rc;
}

void SnakeApp::drawField(GameInfo_t info) {
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImVec2 p = ImGui::GetCursorScreenPos();

  const float cell_size = 20.0f;  // Size of each cell
  const ImU32 empty_color = IM_COL32(0, 0, 0, 255);
  const ImU32 wall_color = IM_COL32(128, 128, 128, 255);
  const ImU32 food_color = IM_COL32(255, 0, 0, 255);
  const ImU32 snake_color = IM_COL32(0, 255, 0, 255);
  for (int y = 0; y < 22; y++) {
    for (int x = 0; x < 12; x++) {
      int cellValue = info.field[y][x];
      ImU32 cell_color = empty_color;  // Default to empty

      switch (cellValue) {
        case s21::empty:
          cell_color = empty_color;
          break;
        case s21::wall:
          cell_color = wall_color;
          break;
        case s21::food:
          cell_color = food_color;
          break;
        case s21::snake:
          cell_color = snake_color;
          break;
        default:
          break;
      }

      ImVec2 cell_min = ImVec2(p.x + x * cell_size, p.y + y * cell_size);
      ImVec2 cell_max = ImVec2(cell_min.x + cell_size, cell_min.y + cell_size);
      draw_list->AddRectFilled(cell_min, cell_max, cell_color);
    }
  }
}

void SnakeApp::resetGame() {
  menuFlag = true;
  controller->model->_snake.clear();
  controller->model->state = s21::StartGame;
  controller->model->direction = s21::UpDir;
  controller->model->info.score = controller->model->info.level = 0;
  controller->model->info.pause = false;
  controller->model->info.speed = 500;
  controller->model->info.level = 0;
  controller->model->info.high_score = controller->model->getSnakeScore();
}

void SnakeApp::snakeLoop() {
  bool hold = false;
  UserAction_t action = getAction();
  if (action == Action) hold = true;
  controller->userInput(action, hold);
  GameInfo_t display = controller->updateCurrentState();
  drawField(display);
  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 200);
  ImGui::Text("Score: %d", display.score);
  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 200);
  ImGui::Text("Level: %d", display.level);
  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 200);
  ImGui::Text("High score: %d", display.high_score);
}