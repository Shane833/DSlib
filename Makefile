# There are certain predefined variables that are
# involved when performing compilation and linking
# so you can modify them accordingly

# The recipe used for compilation is:
# ‘$(CC) $(CPPFLAGS) $(CFLAGS)-c’

# The recipe used for linking is:
# ‘$(CC) $(LDFLAGS) n.o $(LOADLIBES) $(LDLIBS)’

CFLAGS = -Wall -Wextra -ggdb -Isrc -std=c23 -Wpedantic -DNDEBUG
# The two options below will only be used in the linking phase
LDFLAGS = -Lbuild
LDLIBS = -lds

ifeq ($(OS), Windows_NT)
SHELL=cmd.exe
endif

SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

TESTS_SRC = $(wildcard tests/*.c)
TESTS = $(patsubst %.c,%,$(TESTS_SRC))

TARGET = build/libds.a

all: build $(TARGET) tests

$(TARGET): CFLAGS+=-fPIC
$(TARGET): build $(OBJ)
	ar rcs $@ $(OBJ)
	ranlib $@

ifeq ($(OS), Windows_NT)
tests: $(TESTS)
	powershell -ExecutionPolicy Bypass -File ./run_tests.ps1
else
tests: $(TESTS)
	./run_tests.sh
endif

build:
	-mkdir build

ifeq ($(OS), Windows_NT)
clean:
	-del /Q src\\*.o
	-del /Q build\\*.a
	-del /Q tests\\*.exe
else
clean:
	-rm $(OBJ)
	-rm $(TARGET)
	-rm $(TESTS)
endif
