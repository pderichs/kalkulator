CC=clang
CXX=clang++
RM=rm -f

SRC_DIR=src
TESTS_DIR=src/tests
TARGET_DIR=bin
TOOLS_DIR=src/tools

CPPFLAGS=-std=c++17 -O3 -Wall -Wpedantic -Wextra -Weffc++ -I$(TOOLS_DIR) -I$(TESTS_DIR)
LDFLAGS=
LDLIBS=

SRCS=$(SRC_DIR)/main.cpp
SRCS+=$(TOOLS_DIR)/tools.cpp
SRCS+=$(TESTS_DIR)/tests.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

TARGET=$(TARGET_DIR)/target

.PHONY: all
all: $(TARGET)

.PHONY: run
run: $(TARGET)
	sh -c "$(TARGET)"

# .PHONY: test
# test: $(TARGET)
# 	sh -c "$(TARGET) test"

.PHONY: unittests
unittests: $(TARGET)
	sh -c "$(TARGET) unittests"

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(SRC_DIR)/tests.o: $(TESTS_DIR)/tests.cpp $(TESTS_DIR)/tests.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)

.PHONY: checkleak
checkleak: $(TARGET)
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			$(TARGET) test

.PHONY: callgrind
callgrind: $(TARGET)
	valgrind --tool=callgrind $(TARGET) test

.PHONY: debug
debug: $(TARGET)
	ddd $(TARGET)
