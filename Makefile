CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -fsanitize=address
CLIBS = -lm

SOURCES = main.c functions.c
TEST_SOURCES = tests.c functions.c

EXECUTABLE = main
TEST_EXECUTABLE = tests

.PHONY: all test clean unit_test integration_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	@$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(CLIBS)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	@$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXECUTABLE) $(CLIBS)

clean:
	@rm -f $(EXECUTABLE) $(TEST_EXECUTABLE)

unit_test: $(TEST_EXECUTABLE)
	@echo "=== Running unit tests ==="
	@./$(TEST_EXECUTABLE)
	@echo "=== Unit tests completed ==="
	@echo ""

integration_test: $(EXECUTABLE)
	@echo "=== Running integration tests ==="
	@echo "Test 1: Invalid number input"
	@echo "abc" | ./$(EXECUTABLE) 2>/dev/null && echo "ERROR: Program did not return error" || [ $$? -eq 1 ] && echo "SUCCESS: Correct handling of invalid number"

	@echo ""
	@echo "Test 2: Correct processing of numbers"
	@echo -e "3\n1\n2\n3" | ./$(EXECUTABLE) 2>&1 | grep -q "2" && echo "SUCCESS: Correct Prostoe output" || echo "ERROR: Incorrect Prostoe output"

	@echo "=== Integration tests completed ==="

test: unit_test integration_test
	@echo ""
	@echo "=== All tests completed successfully! ==="