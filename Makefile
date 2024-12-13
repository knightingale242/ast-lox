CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

TARGET = $(BIN_DIR)/souse

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	@echo "Running Souse Interpreter..."
	@$(TARGET)

run-script:
	@if [ -z "$(script)" ]; then \
		echo "Please provide a script file: make run-script script=yourscript.souse"; \
	else \
		echo "Running script: $(script)"; \
		$(TARGET) $(script); \
	fi

$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean run run-script