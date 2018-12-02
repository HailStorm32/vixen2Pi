#Path to .h files 
IDIR=include

#Path to .cpp files
SRC_DIR=src

#Path to .o files
OBJ_DIR=$(SRC_DIR)/obj

#All .cpp files
SRC = $(wildcard $(SRC_DIR)/*.cpp)

#All .o files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#All .h files
HEADERS=$(wildcard $(IDIR)/*.h)

GCC=g++
CXXFLAGS=-std=c++17 -g -I$(IDIR)
LDFLAGS=
EXE=vixen2Pi

.PHONY: all clean

#debug variable prints
$(warning $(HEADERS))
$(warning $(OBJ))
$(warning $(SRC))

all: $(EXE)

$(EXE): $(OBJ)
	$(GCC) $(LDFLAGS) $^ -o $@

$(OBJ): $(SRC) $(HEADERS)
	$(GCC) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJ_DIR)/*.o
	rm $(EXE)
