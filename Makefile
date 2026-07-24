# There are certain predefined variables that are
# involved when performing compilation and linking
# so you can modify them accordingly

# The recipe used for compilation is:
# ‘$(CC) $(CPPFLAGS) $(CFLAGS)-c’

# The recipe used for linking is:
# ‘$(CC) $(LDFLAGS) n.o $(LOADLIBES) $(LDLIBS)’

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

debug: CFLAGS = -Wall -Wextra -ggdb -fPIC -Isrc -std=c23 -Wpedantic -DNDEBUG
debug: build $(TARGET) tests

release: CFLAGS = -Isrc -std=c23 -Wpedantic -fPIC -DNDEBUG
release: build $(TARGET)
	-mkdir release
	-mkdir release\\include release\\lib
	-mkdir release\\include\\DS
	-powershell cp src/*.h release/include/DS/
	-powershell cp build/*.a release/lib/

$(TARGET): build $(OBJ)
	ar rcs $@ $(OBJ)
	ranlib $@

ifeq ($(OS), Windows_NT)
tests: CFLAGS =-Wall -Wextra -ggdb -Isrc -std=c23 -Wpedantic 
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
