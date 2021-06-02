CC = g++

COMPILER_FLAGS = -std=c++17 -Wall `Magick++-config --cxxflags --cppflags`

DEBUG = -g -O0 -DDEBUG_BUILD

LINKER_FLAGS = -Llib -L/usr/local/lib -lSPKlog `Magick++-config --ldflags --libs`

INCLUDES = -Iinclude -Ilib -I/usr/local/include -I/usr/local/include/ImageMagick-7

QD_PARSER = quickdeck_parse/parser.tab.cc quickdeck_parse/lexer.yy.c

QD_CPP_SRC = src/main.cpp src/ast.cpp src/canvas.cpp src/environment.cpp src/function_call.cpp src/operator.cpp src/value.cpp $(QD_PARSER)

QD_DEPS = $(QD_CPP_SRC) include/ast.h  include/canvas.h  include/environment.h  include/operator.h  include/parser.h  include/value.h

P_CPP_SRC = param_parse/parser.tab.cc param_parse/lexer.yy.c src/value.cpp

P_DEPS = $(P_CPP_SRC) include/data.h include/value.h

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

quickdeck_parse/lexer.yy.c: quickdeck_parse/lexer.l quickdeck_parse/parser.tab.cc
	flex -o $@ $<

quickdeck_parse/parser.tab.cc: quickdeck_parse/parser.yy
	cd quickdeck_parse && bison -d parser.yy && cd ..

params: $(P_DEPS)
	$(CC) $(COMPILER_FLAGS) -o $@ $(P_CPP_SRC) $(INCLUDES) -Iparam_parse $(LINKER_FLAGS)

params_debug: $(P_DEPS)
	$(CC) $(DEBUG) $(COMPILER_FLAGS) -o $@ $(P_CPP_SRC) $(INCLUDES) -Iparam_parse $(LINKER_FLAGS)

param_parse/lexer.yy.c: param_parse/lexer.l param_parse/parser.tab.cc
	flex -o $@ $<

param_parse/parser.tab.cc: param_parse/parser.yy
	cd param_parse && bison -d parser.yy && cd ..

.PHONY: clean
clean:
	rm quickdeck_parse/parser.tab.cc quickdeck_parse/lexer.yy.c param_parse/parser.tab.cc param_parse/lexer.yy.c quickdeck quickdeck_debug params params_debug quickdeck_parse/*.hh param_parse/*.hh

.PHONY: test
test:
	cd test && ./test.sh
