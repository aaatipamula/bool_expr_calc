CXX = g++
CXXFLAGS = -g -wall

SRC_DIR = src
BIN_DIR = bin

EXE = $(BIN_DIR)/main
SRC = $(wildcard $(SRC_DIR)/*.cpp)
	
.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
    $(CXX) $(CXX) $^ -o $@

$(BIN_DIR) $(OBJ_DIR):
    mkdir -p $@

clean:
    @$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

