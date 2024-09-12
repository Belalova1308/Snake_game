CC = gcc
CPP=g++
CFLAGS = -lncurses -Wall -Wextra -Werror -std=c++17
GTEST=-lgtest
GCOV=-fprofile-arcs -ftest-coverage
CONTROLLER_DIR = brick_game/snake/controller/
MODEL_DIR=brick_game/snake/model/
TETRIS_DIR=brick_game/tetris/
DRAWDIR = gui/cli/
OBJSDIR = OBJS/
OBJS = *.o
SRCS = *.cpp
TARGET = game_cli
MK = mkdir -p
AR = ar rc
RM = rm -rf
RBIN = tests_game gcov_test-test.gcda gcov_test-test.gcno tests_game.info report
G = *.gcno
GA = *.gcda
MV = mv
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
	LEAK_CHECK=valgrind --tool=memcheck --leak-check=yes
	OPEN_CMD=xdg-open
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	LEAK_CHECK=leaks -atExit --
	OPEN_CMD=open
endif

all: clean libgame.a $(OBJS) $(TARGET) game_gui

libgame.a: $(OBJS)
	$(AR) libgame.a $(OBJSDIR)$(OBJS)

$(TARGET):$(OBJS)
	$(CPP) $(DRAWDIR)main.cpp -o $(TARGET) $(OBJSDIR)$(OBJS) $(CFLAGS) -g

$(OBJS): $(DRAWDIR)snakeLoop.cpp $(CONTROLLER_DIR)$(SRCS) $(MODEL_DIR)$(SRCS)
	$(MK) $(OBJSDIR)
	$(CC) -c $(DRAWDIR)screen.c $(TETRIS_DIR)tetris.c
	$(CPP) -c $(DRAWDIR)snakeLoop.cpp $(CONTROLLER_DIR)$(SRCS) $(MODEL_DIR)$(SRCS)
	$(MV) $(OBJS) $(OBJSDIR)

game_gui:
	$(MAKE) -C gui/desktop gameDesktop

test: clean
	$(CPP) $(CFLAGS) $(CONTROLLER_DIR)$(SRCS) $(MODEL_DIR)$(SRCS) tests/*.cpp -o tests_game $(GTEST) -lm
	./tests_game

gcov_report: clean
	$(CPP) $(CFLAGS) $(CONTROLLER_DIR)$(SRCS) $(MODEL_DIR)$(SRCS) tests/*.cpp -o tests_game $(GTEST) -lm $(GCOV)
	chmod +x *
	./tests_game
	lcov -c -d . --no-external -o tests_game.info 
	genhtml -o report tests_game.info
	open ./report/index.html
	open report/gcov_report.html

leaks: clean
	$(CPP) $(CFLAGS) $(CONTROLLER_DIR)$(SRCS) $(MODEL_DIR)$(SRCS) tests/*.cpp -o tests_game $(GTEST) -lm
	$(LEAK_CHECK) ./tests_game

install: all
	mkdir -p build
	$(MV) game_cli build
	$(MV) gui/desktop/gameDesktop build

uninstall:
	rm -rf build

dvi:
	code doc/documentation.md

dist: install
	tar -cvzf game.tgz build

clang:
	clang-format -n $(CONTROLLER_DIR)$(SRCS) $(MODEL_DIR)$(SRCS) tests/*.cpp $(DRAWDIR)*.c $(DRAWDIR)*.cpp gui/desktop/*.cpp

clean:
	$(RM) $(OBJSDIR) $(TARGET) *.a $(RBIN) build
	$(RM) gui/desktop/obj gameDesktop
	$(RM) game.tgz
	$(RM) $(GA)
	$(RM) $(G)