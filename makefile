# Source and object configurations
SRC_DIR := .
OBJ_DIR := .
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# SFML specific paths
SFML_PATH := /opt/homebrew/opt/sfml@2
CPPFLAGS := -g -Wall -std=c++17 -I$(SFML_PATH)/include -pthread
LDFLAGS := -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Output target binary name
TARGET := game.out

# Build executable target
$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Build individual object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

# Run the game
run: $(TARGET)
	./$(TARGET)

# Clean files
clean:
	rm -f $(TARGET) $(OBJ_FILES)