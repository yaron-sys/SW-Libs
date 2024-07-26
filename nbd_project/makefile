#Created: 10-06-18
include .depends

MODULE=nbd_test

CC=clang++
CFLAGS=-Wall -pedantic-errors -Wextra -std=c++11 -D_DEBUG -g
CFLAGS+=-Iinclude
EXTRA_FLAGS=-fPIC -lpthread -ldl -Wl,-rpath=../build -Lbuild -lfactory_handleton_impl

SRC_DIR = src
HEADER_DIR = include
OBJ_DIR = dir

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
#HEADERS=$(wildcard $(HEADER_DIR)/*.hpp)

STAT_LIBS=$(wildcard *.a)
#STAT_LIBS+=build/lib_factory_handleton_impl.so
SHRD_LIBS=$(wildcard *.so)

OBJECTS=$(SRC:.cpp=.o)
#OBJECTS=

#TO_FILTER_OUT=$(MODULE_RELEASE).cpp $(MODULE_RELEASE).o
TO_FILTER_OUT=src/factory_handleton.cpp
SRC=$(filter-out $(TO_FILTER_OUT), $(SOURCES))
#OBJ=$(SRC:.cpp=.o)

MODULE_PATH=$(shell pwd)

EXEC=$(MODULE).out

#MORE_HEADERS=$(wildcard $(MODULE_PATH)/headers/*.hpp)
MORE_HEADERS=

#MORE_TO_RM=$(wildcard $(MODULE_PATH)/headers/*.gch)
MORE_TO_RM=$(wildcard *.gch)

test: $(EXEC)
	mv $(EXEC) ./tests

$(EXEC): .depends shared_obj
	$(CC) $(CFLAGS) $(SRC) $(STAT_LIBS) $(EXTRA_FLAGS) -o $(EXEC)
	rm -f $(MORE_TO_RM)

.PHONY:.depends
.depends: $(SRC) 
	gcc -MM $(CFLAGS) $(SRC) > .depends
	
shared_obj: 
	g++ $(CFLAGS) src/test_user_plugin/user_plugin.cpp -fPIC -shared -o libuser_plugin.so
	mv libuser_plugin.so ./libs
	g++ $(CFLAGS) -fPIC src/factory_handleton.cpp -shared -o libfactory_handleton_impl.so
	mv libfactory_handleton_impl.so ./build

.PHONY:clean
clean:
	rm -f .depends ./tests/$(EXEC) $(OBJECTS) $(SHRD_LIBS) ./libs/* ./build/*
	
	
