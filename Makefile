CC = g++

COMPILER_FLAGS = -std=c++17 -Wall `Magick++-config --cxxflags --cppflags`

DEBUG = -DDEBUG_BUILD

LINKER_FLAGS = -Llib -L/usr/local/lib -lSPKlog `Magick++-config --ldflags --libs`

INCLUDES = -I. -Iinclude -Ilib -I/usr/local/include -I/usr/local/include/ImageMagick-7

CPP_SRC = src/main.cpp src/ast.cpp src/canvas.cpp src/environment.cpp src/function_call.cpp src/operator.cpp src/value.cpp

.PHONY: all
all: build debug

.PHONY: build
build: quickdeck

.PHONY: debug
debug: quickdeck_debug

quickdeck: lex.yy.c parser.tab.cc $(CPP_SRC)
	$(CC) $(COMPILER_FLAGS) -o quickdeck $(CPP_SRC) parser.tab.cc lex.yy.c $(INCLUDES) $(LINKER_FLAGS)

quickdeck_debug: lex.yy.c parser.tab.cc $(CPP_SRC)
	$(CC) $(DEBUG) $(COMPILER_FLAGS) -o quickdeck_debug $(CPP_SRC) parser.tab.cc lex.yy.c $(INCLUDES) $(LINKER_FLAGS)

lex.yy.c: parser.tab.hh src/lexer.l
	flex src/lexer.l

parser.tab.hh: parser.tab.cc
parser.tab.cc: src/parser.yy
	bison -d src/parser.yy	

.PHONY: clean
clean:
	rm *.hh *.cc lex.yy.c
