#include "tetrisImgui.h"

TetrisApp::TetrisApp(){};
TetrisApp::~TetrisApp(){};
const float CELL_SIZE = 20.0f;
ImU32 colors[] = {
    IM_COL32(0, 0, 0, 0),          // 0: empty
    IM_COL32(255, 0, 0, 255),      // 1: green on red
    IM_COL32(0, 255, 0, 255),      // 2: green on green
    IM_COL32(255, 255, 0, 255),    // 3: green on yellow
    IM_COL32(0, 0, 255, 255),      // 4: green on blue
    IM_COL32(255, 165, 0, 255),    // 5: green on magenta
    IM_COL32(255, 192, 203, 255),  // 6: green on cyan
    IM_COL32(128, 0, 128, 255),    // 7: green on white
    IM_COL32(255, 255, 255, 255)   // 8: white on white
};
void TetrisApp::timing(Extra_info *extr) {
  usleep(50 * 1000);
  extr->speed_counter++;
  extr->fall =
      (extr->speed_counter == extr->gameInfo.speed - (extr->gameInfo.level * 2))
          ? 1
          : 0;
}
void TetrisApp::gameLogic(Extra_info *extr) {
  if (extr->fall == 1) {
    // движение вниз
    if (fitRow(*extr, extr->y + 1)) {
      extr->y++;
    } else {
      // фиксация фигура на поле
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
          if (get_cell(*extr, y, x) == 'X') {
            int temp_y = extr->y + y;
            int temp_x = extr->x + x;
            extr->gameInfo.field[temp_y][temp_x] = extr->currentPiece + 1;
          }
        }
      }
      // уничтожение линий
      check_for_rows(extr);
      // генерация новой фигуры
      create_new_piece(extr);
      // проверка на окончание игры
      extr->over = fit(*extr, extr->rotation, extr->y, extr->x) == 0 ? 1 : 0;
    }
    extr->speed_counter = 0;
  }
}
char TetrisApp::get_cell(Extra_info extr, int y, int x) {
  return extr.P[extr.currentPiece][rotate(y, x, extr.rotation)];
}
int TetrisApp::rc2i(int y, int x) { return y * WIDTH + x; }
int TetrisApp::fit(Extra_info extr, int rot, int row, int col) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      int pi = rotate(y, x, rot);
      int fi = rc2i(row + y, col + x);
      if (extr.P[extr.currentPiece][pi] == 'X' &&
          extr.gameInfo.field[fi / WIDTH][fi % WIDTH] != 0) {
        return 0;
      }
    }
  }
  return 1;
}
int TetrisApp::fitCol(Extra_info extr, int col) {
  return fit(extr, extr.rotation, extr.y, col);
}
// Проверка движения влево вправо
int TetrisApp::fitRow(Extra_info extr, int row) {
  return fit(extr, extr.rotation, row, extr.x);
}
void TetrisApp::fillTetris(ImDrawList *draw_list, ImVec2 pos, int color) {
  ImVec2 cell_max = ImVec2(pos.x + CELL_SIZE, pos.y + CELL_SIZE);
  draw_list->AddRectFilled(pos, cell_max, colors[color]);
}
int TetrisApp::fitRot(Extra_info extr, int rot) {
  return fit(extr, rot, extr.y, extr.x);
}
void TetrisApp::create_new_piece(Extra_info *extr) {
  extr->y = 0;
  extr->x = WIDTH / 2;
  extr->rotation = 0;
  extr->currentPiece = extr->nextPiece;
  extr->nextPiece = rand() % 7;
}
void TetrisApp::check_for_rows(Extra_info *extr) {
  int found_line;
  int lines_count = 0;

  for (int y = 0; y < 4; y++) {
    if (extr->y + y < HEIGHT - 1) {
      found_line = 1;
      for (int x = 1; x < WIDTH - 1; x++) {
        if (extr->gameInfo.field[extr->y + y][x] == 0) {
          found_line = 0;
          break;
        }
      }

      if (found_line == 1) {
        lines_count++;
        for (int ly = (extr->y + y) - 1; ly > 0; ly--) {
          for (int lx = 1; lx < WIDTH - 1; lx++) {
            extr->gameInfo.field[ly + 1][lx] = extr->gameInfo.field[ly][lx];
          }
        }
      }
    }
  }
  if (lines_count == 1)
    extr->gameInfo.score += 100;
  else if (lines_count == 2)
    extr->gameInfo.score += 300;
  else if (lines_count == 3)
    extr->gameInfo.score += 700;
  else if (lines_count == 4)
    extr->gameInfo.score += 1500;
  if (extr->gameInfo.score >= extr->gameInfo.level * 600) {
    extr->gameInfo.level++;
  }
}
int TetrisApp::rotate(int y, int x, int type) {
  switch (type % 4) {
    case 0:
      return y * 4 + x;
    case 1:
      return 12 + y - (x * 4);
    case 2:
      return 15 - (y * 4) - x;
    case 3:
      return 3 - y + (x * 4);
  }
  return 0;
}
void TetrisApp::initializeGameInfo(Extra_info *extr) {
  extr->gameInfo.score = 0;
  extr->gameInfo.high_score = 0;
  extr->gameInfo.level = 1;
  extr->gameInfo.speed = 22;
  extr->gameInfo.pause = 0;
  extr->currentPiece = 0;
  extr->nextPiece = 0;
  extr->rotation = 0;
  extr->x = 3;
  extr->y = 0;
  extr->over = 0;
  extr->speed_counter = 0;
  extr->fall = 0;
  strcpy(extr->P[0], "..X...X...X...X.");
  strcpy(extr->P[1], "..X..XX..X......");
  strcpy(extr->P[2], ".X...XX...X.....");
  strcpy(extr->P[3], ".....XX..XX.....");
  strcpy(extr->P[4], "..X..XX...X.....");
  strcpy(extr->P[5], "..X...X..XX.....");
  strcpy(extr->P[6], ".XX...X...X.....");
}
void TetrisApp::drawTetris(Extra_info extr) {
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImVec2 p = ImGui::GetCursorScreenPos();

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int cellValue = extr.gameInfo.field[y][x];
      ImVec2 cell_pos = ImVec2(p.x + x * CELL_SIZE, p.y + y * CELL_SIZE);
      ImVec2 cell_max = ImVec2(cell_pos.x + CELL_SIZE, cell_pos.y + CELL_SIZE);
      switch (cellValue) {
        case 0:
          // Draw empty space
          draw_list->AddRectFilled(cell_pos, cell_max, colors[0]);
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          fillTetris(draw_list, cell_pos, cellValue);
          break;
        case 9:
          fillTetris(draw_list, cell_pos, 8);
          break;
        default:
          break;
      }
    }
  }

  // Draw the current piece
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (get_cell(extr, y, x) == 'X') {
        ImVec2 cell_pos = ImVec2(p.x + (extr.x + x) * CELL_SIZE,
                                 p.y + (extr.y + y) * CELL_SIZE);
        fillTetris(draw_list, cell_pos, extr.currentPiece + 1);
      }
    }
  }
  makeNext(extr);
  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 200);
  ImGui::Text("Score: %d", extr.gameInfo.score);
  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 200);
  ImGui::Text("Level: %d", extr.gameInfo.level);
}

void TetrisApp::makeNext(Extra_info extr) {
  int startX = (20 - 4) / 2;
  int startY = 3;

  // Initialize the next piece field
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 20; x++) {
      extr.gameInfo.next[y][x] =
          (x == 0 || x == 19 || y == 9 || y == 0) ? 9 : 0;
    }
  }

  // Set the next piece in the field
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      int cellValue = extr.P[extr.nextPiece][rotate(y, x, 0)];
      if (cellValue == 'X') {
        extr.gameInfo.next[startY + y][startX + x] = extr.nextPiece + 1;
      }
    }
  }

  // Get the draw list to draw custom shapes
  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  // Get the window position to start drawing
  ImVec2 p = ImGui::GetCursorScreenPos();

  // Draw the next piece
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 20; x++) {
      int cellValue = extr.gameInfo.next[y][x];
      ImVec2 cell_pos =
          ImVec2(p.x + x * CELL_SIZE + 250, p.y + y * CELL_SIZE + 200);
      ImVec2 cell_max =
          ImVec2(cell_pos.x + CELL_SIZE + 250, cell_pos.y + CELL_SIZE + 200);

      switch (cellValue) {
        case 0:
          // Draw empty space
          draw_list->AddRectFilled(cell_pos, cell_max, colors[0]);
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          fillTetris(draw_list, cell_pos, cellValue);
          break;
        case 9:
          fillTetris(draw_list, cell_pos, 8);
          break;
        default:
          break;
      }
    }
  }
}

void TetrisApp::fieldMem(Extra_info *extr) {
  extr->gameInfo.field = new int *[HEIGHT];
  for (int i = 0; i < HEIGHT; i++) {
    extr->gameInfo.field[i] = new int[WIDTH]();
    for (int j = 0; j < WIDTH; j++) {
      extr->gameInfo.field[i][j] = 0;
    }
  }
  extr->gameInfo.next = new int *[HEIGHT];
  for (int i = 0; i < HEIGHT; i++) {
    extr->gameInfo.next[i] = new int[WIDTH]();
    for (int j = 0; j < WIDTH; j++) {
      extr->gameInfo.next[i][j] = 0;
    }
  }
  extr->nextPiece = rand() % 7;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      extr->gameInfo.field[y][x] =
          (x == 0 || x == WIDTH - 1 || y == HEIGHT - 1) ? 9 : 0;
    }
  }
}
void TetrisApp::tetrisLoop(Extra_info *extr) {
  // fieldMem(&extr);
  // initializeGameInfo(&extr);
  UserAction_t action = getAct();
  timing(extr);
  userInput(action, 0, extr);
  gameLogic(extr);
  drawTetris(*extr);
}

UserAction_t TetrisApp::getAct() {
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

void TetrisApp::userInput(UserAction_t action, bool hold, Extra_info *extr) {
  if (hold) {
    printf("1");
  }
  switch (action) {
    case Left:
      if (fitCol(*extr, extr->x - 1) == 1) extr->x--;
      break;
    case Right:
      if (fitCol(*extr, extr->x + 1) == 1) extr->x++;
      break;
    case Down:
      while (fitRow(*extr, extr->y + 1) == 1) extr->y++;
      break;
    case Action:
      if (fitRot(*extr, extr->rotation + 1) == 1) extr->rotation++;
      break;
    case Pause:
      // pauseMenu();
      break;
    case Terminate:
      extr->over = 1;
      break;
    default:
      break;
  }
}

void TetrisApp::freeField(Extra_info *extr) {
  for (int i = 0; i < 10; i++) delete[] extr->gameInfo.next[i];
  delete[] extr->gameInfo.next;
  for (int i = 0; i < 10; i++) delete[] extr->gameInfo.field[i];
  delete[] extr->gameInfo.field;
}