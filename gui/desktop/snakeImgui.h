#include <stdio.h>

#include "../../brick_game/snake/controller/controller.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif
class SnakeApp {
 public:
  bool menuFlag = true;
  bool snake = false;
  bool tetris = false;
  s21::Controller *controller;
  SnakeApp(s21::Controller *controller);
  ~SnakeApp();
  UserAction_t getAction();
  void drawField(GameInfo_t info);
  void resetGame();
  void snakeLoop();
};