CXX = g++
CXXFLAGS = -g -Wall

SRC_DIR = src
BIN_DIR = bin

EXE = $(BIN_DIR)/lcalc
# SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC= $(filter-out src/Evaluator.cpp src/Tokenizemodule.cpp src/Interpreter.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)
	
.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR)

-include $(OBJ:.o=.d)

