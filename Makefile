# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++14 -Iinc

# Source and header files
SRC_ENCODER = $(wildcard encoder.cpp src/huffman.cpp src/burrows_wheeler.cpp src/move_to_front.cpp src/run_length_encoding.cpp src/utils.cpp)
SRC_DECODER = $(wildcard decoder.cpp src/huffman.cpp src/burrows_wheeler.cpp src/move_to_front.cpp src/run_length_encoding.cpp src/utils.cpp)
HEADERS = $(wildcard inc/*.h)

# Output directory for object files
BUILD_DIR = build

# Target executables
EXEC_ENCODER = bzip2_compress
EXEC_DECODER = bzip2_decompress

# Object files
OBJ_ENCODER = $(SRC_ENCODER:src/%.cpp=$(BUILD_DIR)/%.o)
OBJ_DECODER = $(SRC_DECODER:src/%.cpp=$(BUILD_DIR)/%.o)

# Rules
all: $(EXEC_ENCODER) $(EXEC_DECODER)

$(EXEC_ENCODER): $(OBJ_ENCODER)
	$(CXX) $(OBJ_ENCODER) -o $(EXEC_ENCODER)

$(EXEC_DECODER): $(OBJ_DECODER)
	$(CXX) $(OBJ_DECODER) -o $(EXEC_DECODER)

$(BUILD_DIR)/%.o: src/%.cpp $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(EXEC_ENCODER) $(EXEC_DECODER)

.PHONY: all clean
