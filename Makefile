CC = g++

COMPILER_FLAGS = -std=c++17 -Wall `Magick++-config --cxxflags --cppflags`

DEBUG = -g -O0 -DDEBUG_BUILD

LINKER_FLAGS = -Llib -L/usr/local/lib -lSPKlog `Magick++-config --ldflags --libs`

INCLUDES = -I. -Iinclude -Ilib -I/usr/local/include -I/usr/local/include/ImageMagick-7

QD_CPP_SRC = src/main.cpp src/ast.cpp src/canvas.cpp src/environment.cpp src/function_call.cpp src/operator.cpp src/value.cpp script.yy.c script.tab.cc

QD_DEPS = $(QD_CPP_SRC) include/ast.h  include/canvas.h  include/environment.h  include/operator.h  include/parser.h  include/value.h quickdeck_parse/location.hh quickdeck_parse/position.hh quickdeck_parse/stack.hh

P_CPP_SRC = data.tab.cc data.yy.c src/value.cpp

P_DEPS = $(P_CPP_SRC) include/data.h include/value.h param_parse/location.hh param_parse/position.hh param_parse/stack.hh

.PHONY: all
all: build debug

.PHONY: build
build: quickdeck params

.PHONY: debug
debug: quickdeck_debug params_debug

quickdeck: $(QD_DEPS)
	$(CC) $(COMPILER_FLAGS) -o $@ $(QD_CPP_SRC) $(INCLUDES) -Iquickdeck_parse $(LINKER_FLAGS)

quickdeck_debug: $(QD_DEPS)
	$(CC) $(DEBUG) $(COMPILER_FLAGS) -o $@ $(QD_CPP_SRC) $(INCLUDES) -Iquickdeck_parse $(LINKER_FLAGS)

script.tab.cc script.yy.c quickdeck_parse/location.hh quickdeck_parse/position.hh quickdeck_parse/stack.hh: quickdeck_parse/script.yy quickdeck_parse/script.l
	cd quickdeck_parse && make parser;

params: $(P_DEPS)
	$(CC) $(COMPILER_FLAGS) -o $@ $(P_CPP_SRC) $(INCLUDES) -Iparam_parse $(LINKER_FLAGS)

params_debug: data.yy.c data.tab.cc
	$(CC) $(DEBUG) $(COMPILER_FLAGS) -o $@ $(P_CPP_SRC) $(INCLUDES) -Iparam_parse $(LINKER_FLAGS)

data.tab.cc data.yy.c param_parse/location.hh param_parse/position.hh param_parse/stack.hh: param_parse/data.yy param_parse/data.l
	cd param_parse && make parser;

.PHONY: clean
clean:
	rm *.tab.hh *.tab.cc *.yy.c quickdeck quickdeck_debug params params_debug quickdeck_parse/*.hh param_parse/*.hh
