#define WIDTH 12
#define HEIGHT 21
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../../brick_game/common/common.h"
#include "imgui.h"
typedef struct {
  GameInfo_t gameInfo;
  int currentPiece;
  int nextPiece;
  int rotation;
  int x;
  int y;
  int over;
  int speed_counter;
  int fall;
  char P[7][16];
} Extra_info;

class TetrisApp {
 public:
  TetrisApp();
  ~TetrisApp();
  void tetrisLoop(Extra_info* extr);
  void timing(Extra_info* extr);
  void gameLogic(Extra_info* extr);
  char get_cell(Extra_info extr, int y, int x);
  int rc2i(int y, int x);
  int fit(Extra_info extr, int rot, int row, int col);
  int fitCol(Extra_info extr, int col);
  int fitRow(Extra_info extr, int row);
  int fitRot(Extra_info extr, int rot);
  void create_new_piece(Extra_info* extr);
  void check_for_rows(Extra_info* extr);
  int rotate(int y, int x, int type);
  void initializeGameInfo(Extra_info* extr);
  void drawTetris(Extra_info extr);
  void fillTetris(ImDrawList* draw_list, ImVec2 pos, int color);
  void makeNext(Extra_info extr);
  UserAction_t getAct();
  void userInput(UserAction_t action, bool hold, Extra_info* extr);
  void fieldMem(Extra_info* extr);
  void freeField(Extra_info* extr);
};