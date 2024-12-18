# make clean
# make all
#
# custom variant since the test building of Zed's example did not work
# and made me angry

# and since it did not work out, rather use linux for all of it:
# docker run -it --volume D:\Docs\Code\try-c-hardway\:/tmp/app llvm-neovim:latest

CC=clang
CFLAGS=-g -Wall -Wextra
LDFLAGS=-lm

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
TEST_SRC=$(wildcard tests/*_tests.c)
TEST_OBJ=$(patsubst %.c,%.o,$(TEST_SRC))
TESTS=$(patsubst %.c,%,$(TEST_SRC))

all: clean main tests execute_tests

build: main tests

main: $(OBJECTS)
	@echo "===\nBuilding default target rule\n==="
	@echo $(SOURCES)
	@echo $(OBJECTS)


# rule to build .o from .c files
# if like "main:" something depends on $(OBJECTS), 
# then make applies this implicit rule automatically
%.o: %.c
	@echo "c to the o (" $< ")\n"
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(TEST_OBJ) $(TESTS)
	@echo "===\nBuilding tests\n==="

# test executable rule
#  (last custom rule only worked for one test executable file)
# % matches test name
# implicitly called by tests: rule, since it depends on the TESTS
%: %.o $(OBJECTS)
	@echo "test executable rule: $@"
	$(CC) $(LDFLAGS) -o $@ $^

execute_tests:
	@echo "===\nRun tests\n==="
	./tests/runtests.sh

clean:
	@echo "===\nCleaning\n==="
	rm -rf build $(OBJECTS) $(TESTS) $(TEST_OBJ)

