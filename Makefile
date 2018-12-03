#Path to .h files 
IDIR=include

#Path to .cpp files
SRC_DIR=src

#Path to .o files
OBJ_DIR=$(SRC_DIR)/obj

#Path to libraries
LDIR=contrib

#Path to wiringPi lib
WIR_PI_DIR=$(LDIR)/wiringPi/wiringPi

#All .cpp files
SRC = $(wildcard $(SRC_DIR)/*.cpp)

#All .o files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#All .h files
HEADERS=$(wildcard $(IDIR)/*.h)

TARGET=#arm-linux-gnueabihf-
CXX=$(TARGET)g++
CC=$(TARGET)gcc
CXXFLAGS=-std=c++17 -g -I$(IDIR) -I$(WIR_PI_DIR)
LDFLAGS= -L$(WIR_PI_DIR) -lwiringPi
EXE=vixen2Pi

.PHONY: all clean wiringPi

#debug variable prints
$(warning $(HEADERS))
$(warning $(OBJ))
$(warning $(SRC))
#$(warning $(LDFLAGS))

all: $(EXE)

$(EXE): $(OBJ) | wiringPi
	ln -s ~/vixen2Pi/$(WIR_PI_DIR)/libwiringPi.so* ~/vixen2Pi/$(WIR_PI_DIR)/libwiringPi.so
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

wiringPi:
	make -C $(WIR_PI_DIR) CC=$(CC)

clean:
	make -C $(WIR_PI_DIR) clean
	rm $(OBJ_DIR)/*.o
	rm $(EXE)
