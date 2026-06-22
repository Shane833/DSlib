CFLAGS = -Wall -Wextra -ggdb -Isrc -std=c23 -Wpedantic -DNDEBUG

ifeq ($(OS), Windows_NT)
SHELL=cmd.exe
endif

SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

TESTS_SRC = $(wildcard tests/*.c)
TESTS = $(patsubst %.c,%,$(TESTS_SRC))

TARGET = build/libds.a

all: $(TARGET)

$(TARGET): CFLAGS+=-fPIC
$(TARGET): build $(OBJ)
	ar rcs $@ $(OBJ)
	ranlib $@

build:
	-mkdir build

remove:
	-rmdir /S /Q build
