# make -f Makefile2 clean
# make -f Makefile2 all
#
# custom variant since the test building of Zed's example did not work
# and made me angry

CFLAGS=-g -Wall -Wextra -Isrc

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
TEST_SRC=$(wildcard tests/*_tests.c)
TEST_OBJ=$(patsubst %.c,%.o,$(TEST_SRC))
TESTS=$(patsubst %.c,%,$(TEST_SRC))

all: clean main tests execute_tests

build: main tests

main: $(OBJECTS)
	@echo "===\nBuilding default target\n==="
	@echo $(SOURCES)
	@echo $(OBJECTS)
	clang $(CFLAGS) -c $(SOURCES) -o $(OBJECTS)

tests: $(TEST_OBJ)
	@echo "===\nBuilding tests\n==="
	@echo $(TEST_SRC)
	@echo $(TESTS)
	clang $(CFLAGS) -c $(TEST_SRC) -o $(TEST_OBJ)
	clang $(CFLAGS) $(OBJECTS) $(TEST_OBJ) -o $(TESTS)

execute_tests:
	@echo "===\nRun tests\n==="
	./tests/runtests.sh

clean:
	@echo "===\nCleaning\n==="
	rm -rf build $(OBJECTS) $(TESTS) $(TEST_OBJ)

