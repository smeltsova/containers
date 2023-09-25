CC = g++

ASAN = #-fsanitize=address
CFLAGS = -c -g -std=c++17 #-Wall -Wextra -Werror 
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

TEST_FLAGS = -lgtest
ifeq ($(shell uname), Linux)
TEST_FLAGS = -lgtest -lm -lpthread -lrt -lsubunit
endif

TEST_SRC_DIR = ./tests
TEST_OBJ_DIR = ./tests/objs
TEST_SRC:=$(shell find $(TEST_SRC_DIR) -name "*.cc")
TEST_OBJS:=$(addprefix $(TEST_OBJ_DIR)/, $(notdir $(TEST_SRC:.cc=.o)))

LIB_SRC:=$(shell find . -maxdepth 1 -name "*.cc")
LIB_OBJS:=$(LIB_SRC:.cc=.o)

TARGET= #s21_matrix_oop.a 

LIB_OBJS_GCOV:=$(LIB_SRC:.cc=_gcov.o)

TARGET_GCOV=s21_matrix_oop_gcov.a 

all: $(TARGET)

$(TARGET): $(LIB_OBJS)
	ar rcs $(TARGET) $(LIB_OBJS)
	ranlib $(TARGET)

$(LIB_OBJS): %.o: %.cc
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

test: clean $(TARGET) $(TEST_OBJS)
	$(CC) $(ASAN) $(TEST_OBJS) -o test $(TEST_FLAGS) -L. $(TARGET)
	./test

$(TEST_OBJS): $(TEST_OBJ_DIR)%.o: $(TEST_SRC_DIR)%.cc
	mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

gcov_report: clean coverage.html open

coverage.html: gcov_test
	gcovr -r . --html-details -o coverage.html

gcov_test: $(TEST_OBJS) $(TARGET_GCOV)
	$(CC) $(TEST_OBJS) $(TARGET_GCOV) $(TEST_FLAGS) $(GCOV_FLAGS)  -o gcov_test
	./gcov_test

$(TARGET_GCOV): $(LIB_OBJS_GCOV)
	ar rcs $(TARGET_GCOV) $(LIB_OBJS_GCOV)
	ranlib $(TARGET_GCOV)

$(LIB_OBJS_GCOV): %_gcov.o: %.cc
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(ASAN) $< -o $@

open:
	open coverage.html

clean:
	rm -rf *.o $(TARGET) test_$(TARGET) test gcov_test $(TEST_OBJ_DIR)/*.o *.gcno *.gcda *.gcov *gcov.a coverage* $(TEST_OBJ_DIR)/*.gcno  $(TEST_OBJ_DIR)/*.gcda  $(TEST_OBJ_DIR)/*.gcov *.gz
 
rebuild: clean all

valgrind: clean test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test

check: test
	cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem *.h *.cc ./tests/*.cc ./tests/*.h
	cp ../materials/linters/.clang-format .
	clang-format -n *.cc ./tests/*.cc
	CK_FORK=no leaks --atExit -- ./test