  
CPPFLAGS += -std=c++17 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include -I src

HPPFILES := $(shell find include/ -type f -name "*.hpp")
CPPFILES := $(shell find src/ -type f -name "*.cpp")
OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))

all : bin/c_compiler

src/lexer.yy.cpp : src/lexer.flex src/parser.tab.hpp
	flex -o src/lexer.yy.cpp  src/lexer.flex

src/parser.tab.cpp src/parser.tab.hpp : src/parser.y
	bison -v -d src/parser.y -o src/parser.tab.cpp


src/%.o: src/%.cpp $(HPPFILES)
	g++ $(CPPFLAGS) -c -o $@ $<

bin/compiler : src/lexer.yy.o src/parser.tab.o $(OBJS)
	mkdir -p bin
	g++ $(CPPFLAGS) $^ -o $@


bin/c_compiler : bin/compiler src/wrapper.sh
	cp src/wrapper.sh bin/c_compiler
	chmod u+x bin/c_compiler

.PHONY: clean

clean :
	rm -rf bin/*
	rm -rf out/*
	rm -f src/*.tab.hpp
	rm -f src/*.tab.cpp
	rm -f src/*.yy.cpp
	rm -f src/*.output
	find src/ -type f -name '*.o' -delete
	find src/ -type f -name '*.o' -delete
	find -type f -name '*.o' -delete
	find -type f -name '*.s' -delete

