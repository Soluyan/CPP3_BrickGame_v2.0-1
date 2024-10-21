PROJECT_NAME = s21_BrickGame
PROJECT_VERSION = 1_0

CC = gcc
CC+ = g++
CPPFLAGS = -Wall -Wextra -std=c++17
# CPPFLAGS += -Werror
CPPFLAGS += -g
# CPPFLAGS += -fsanitize=address -g

.PHONY: test

UNAME_S = $(shell uname)

ifeq ($(UNAME_S),Darwin)
OPEN_CMD = open
endif

ifeq ($(UNAME_S),Linux)
OPEN_CMD = xdg-open
ADD_LDFLAGS = -lm -lsubunit
endif

all: install_snake_term gcov_report

uninstall:
	rm -rf build/

############ TARGET SECTION ###############
BACK_SNAKE=$(wildcard brick_game/snake/*.cpp)
BACK_TETRIS=$(wildcard brick_game/tetris/*.cpp)
OBJ_SNAKE=$(BACK_SNAKE:.cpp=.o)
OBJ_TETRIS=$(BACK_TETRIS:.cpp=.o)

COMMON_SRC = brick_game/common/common.cpp
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)

############ TARGET SECTION ###############



############ BUILD SECTION ################
BUILD_PATH = build

mkbuild:
	rm -rf build/
	mkdir build

############ BUILD SECTION ################



############ LIBRARIES SECTION ############
SNAKE_LIB_STATIC = libsnake.a
TETRIS_LIB_STATIC = libtetris.a
GUI_LIB_STATIC = libgui.a
DESK_LIB_STATIC = libdesk.a

LDFLAGS = $(shell pkg-config --cflags --libs gtk4)

back_lib_snake : $(OBJ_SNAKE) $(COMMON_OBJ)
	ar rc $(BUILD_PATH)/$(SNAKE_LIB_STATIC) $^
	ranlib $(BUILD_PATH)/$(SNAKE_LIB_STATIC) 
	rm brick_game/snake/*.o
	rm brick_game/common/*.o

back_lib_tetris : $(OBJ_TETRIS) $(COMMON_OBJ)
	ar rc $(BUILD_PATH)/$(TETRIS_LIB_STATIC) $^
	ranlib $(BUILD_PATH)/$(TETRIS_LIB_STATIC)
	rm brick_game/tetris/*.o
	rm brick_game/common/*.o

front_lib_gui:
	$(CC+) $(CPPFLAGS) -D SNAKE_CLI -c gui/cli/gui.cpp -o gui/cli/gui.o
	ar rc $(BUILD_PATH)/$(GUI_LIB_STATIC) gui/cli/gui.o
	ranlib $(BUILD_PATH)/$(GUI_LIB_STATIC)
	rm  gui/cli/*.o

front_lib_gui_tetris:
	$(CC+) $(CPPFLAGS) -D TETRIS_CLI -c gui/cli/gui.cpp -o gui/cli/gui.o
	ar rc $(BUILD_PATH)/$(GUI_LIB_STATIC) gui/cli/gui.o
	ranlib $(BUILD_PATH)/$(GUI_LIB_STATIC)
	rm  gui/cli/*.o

desktop_lib_gui:
	$(CC+) $(CPPFLAGS) -D SNAKE_DESK -c gui/desktop/desktop.cpp -o gui/desktop/desktop.o $(LDFLAGS)
	ar rc $(BUILD_PATH)/$(DESK_LIB_STATIC) gui/desktop/desktop.o
	ranlib $(BUILD_PATH)/$(DESK_LIB_STATIC)
	rm  gui/desktop/*.o

############ LIBRARIES SECTION ############



############ SNAKE SECTION #################
SNAKE_TERM_MAKE=2
SNAKE_MODE=1
SNAKE_TERM_PATH = build/snake_term
SNAKE_DESK_PATH = build/snake_desk

snake_t:
	cd build && ./snake_term

snake_d:
	cd build && ./snake_desk

bool_cli_snake: 
	$(MAKE) SNAKE_TERM_MAKE=1 install_snake_term

install_snake_term: mkbuild back_lib_snake front_lib_gui
	$(CC+) $(CPPFLAGS) -D SNAKE_CLI -L $(BUILD_PATH) s21_snake.cpp -o $(SNAKE_TERM_PATH) -lncurses -lsnake -lgui

install_snake_desk: mkbuild back_lib_snake front_lib_gui
	$(CC+) $(CPPFLAGS) -D SNAKE_DESK -L $(BUILD_PATH) gui/desktop/desktop.cpp -o $(SNAKE_DESK_PATH) -lncurses -lsnake $(LDFLAGS)


snake_leaks: mkbuild install_snake_term
	cd build && valgrind --tool=memcheck --leak-check=yes ./snake_term

snake_leaks_desk: mkbuild install_snake_desk
	cd build && valgrind --tool=memcheck --leak-check=yes --log-file=valgrind-out.txt ./snake_desk

uninstall_snake_term:
	rm -rf $(SNAKE_TERM_PATH)
	rm -f snake_score.txt
############ SNAKE SECTION #################



############ TETRIS SECTION #################
TETRIS_TERM_MAKE=2	
TETRIS_MODE=1
TETRIS_TERM_PATH = build/tetris_term
TETRIS_DESK_PATH = build/tetris_desk

tetris_t:
	cd build && ./tetris_term
tetris_d:
	cd build && ./tetris_desk

bool_cli_tetris:
	$(MAKE) TETRIS_TERM_MAKE=1 install_tetris_term

install_tetris_term: mkbuild back_lib_tetris front_lib_gui_tetris
	$(CC+) $(CPPFLAGS) -D TETRIS_CLI -L $(BUILD_PATH) s21_tetris.cpp -o $(TETRIS_TERM_PATH) -lncurses -ltetris -lgui

install_tetris_desk: mkbuild back_lib_tetris front_lib_gui_tetris
	$(CC+) $(CPPFLAGS) -D TETRIS_DESK -L $(BUILD_PATH) gui/desktop/desktop.cpp -o $(TETRIS_DESK_PATH) -lncurses -ltetris $(LDFLAGS)

tetris_leaks: mkbuild install_snake_term
	cd build && valgrind --tool=memcheck --leak-check=yes ./tetris_term

uninstall_tetris_term:
	rm -rf $(TETRIS_TERM_PATH)
	rm -f tetris_score.txt
############ TETRIS SECTION #################



############ GCOVR SECTION ############

gcov_report: add_coverage_flag tests
	./$(BUILD_PATH)/tests_snake
	lcov -t "check" -o check.info -c -d ./  --no-external $(LCOVFLAGS)
	genhtml -o report check.info $(LCOVFLAGS)
	open report/index.html

add_coverage_flag:
# $(eval CPPFLAGS += --coverage)
	$(eval CPPFLAGS += --coverage -lgcov)



############ GCOVR SECTION ############



############ TEST SECTION #############
TESTFLAGS=-lgtest -lpthread

tests: mkbuild back_lib_snake
	echo 0 > $(BUILD_PATH)/snake_score.txt
	$(CC+) $(CPPFLAGS) -L $(BUILD_PATH) tests/tests.cpp -o $(BUILD_PATH)/tests_snake -lncurses -lsnake $(TESTFLAGS)

test_run: test
	./$(BUILD_PATH)/tests_snake

clean_test:
	rm -f $(BUILD_PATH)/tests_snake

############ TEST SECTION #############



############ DVI SECTION ############
DOXYGEN_FILE = doc/Doxyfile
DOCUMENTATION_FILE_NAME = $(PROJECT_NAME)_$(PROJECT_VERSION)_doc
DOCUMENTATION_FILE_HTML = $(DOCUMENTATION_FILE_NAME).html
DOCUMENTATION_FILE_PDF = $(DOCUMENTATION_FILE_NAME).pdf

dvi: documentation
	$(MAKE) -C doc/latex pdf || true
	mv doc/latex/refman.pdf $(DOCUMENTATION_FILE_PDF)

documentation:
	doxygen doc/Doxyfile
	ln -fs doc/html/index.html $(DOCUMENTATION_FILE_HTML)

clean_dvi: clean_dvi_artefacts
	$(RM) $(DOCUMENTATION_FILE_HTML)
	$(RM) $(DOCUMENTATION_FILE_PDF)

clean_dvi_artefacts:
	$(RM) -r doc/html
	$(RM) -r doc/latex
############ DVI SECTION ############



############ DIST SECTION ############
DIST_DIR = dist
dist:
	mkdir -p $(DIST_DIR)
	cp -a brick_game $(DIST_DIR)
	cp -a gui $(DIST_DIR)
	cp -a tests $(DIST_DIR)
	cp -a *.cpp $(DIST_DIR)
	cp -a *.h $(DIST_DIR)
	tar -czf s21_BrickGame.tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)

clear_dist:
	rm -f s21_BrickGame.tar.gz
	rm -rf dist

############ DIST SECTION ############	



############ FORMAT SECTION ############
clang:
	cp ../materials/linters/.clang-format .
	clang-format -i *.c *.h */*.cpp */*.hpp */*/*.cpp */*/*.hpp
	rm .clang-format

check:
	cp ../../../materials/linters/.clang-format ../../brick_game/tetris/.clang-format
	clang-format -n *.c *.h */*.cpp */*.hpp */*/*.cpp */*/*.hpp
	clang-format -n ../../test/*.cpp ../../test/*.hpp
	rm -f .clang-format

############ FORMAT SECTION ############

dep_install_gui:
	sudo apt update
	sudo apt install libgtk-4-1 libgtk-4-common libgtk-4-dev

leaks_test: test
	cd ../../test && leaks -atExit -- ./test

uninstall_all:
	rm -rf build
	rm -f snake_score.txt
	rm -f tetris_score.txt

clear_doc:
	rm -f doc/html/*
	rm -f doc/html/search/*.js
	rm -f doc/latex/*
	rm -f *.html

clear_gc:
	rm  brick_game/common/*.gcda
	rm  brick_game/common/*.gcno
	rm  brick_game/snake/*.gcda
	rm  brick_game/snake/*.gcno

clean: uninstall_all
	rm -f tests/tests_snake && rm -f *.txt
	rm -f *.txt && rm -f *.out
	rm -rf report
	rm -f s21_Tetris.tar.gz