#Path to .h files 
IDIR=include

#Path to .cpp files
SRC_DIR=src

#Path to .o files
OBJ_DIR=$(SRC_DIR)/obj

#Path to cross compiler libraries
RPI_LIB_DIR=/usr/arm-linux-gnueabihf/lib

#Path to wiringPi lib
WIR_PI_DIR=contrib/wiringPi/wiringPi

#All .cpp files
SRC = $(wildcard $(SRC_DIR)/*.cpp)

#All .o files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#All .h files
HEADERS=$(wildcard $(IDIR)/*.h)

WIRPI_LIBS =-L$(RPI_LIB_DIR) -lm -lpthread -lrt -lcrypt -lssl -lcrypto -lz -ldl
TARGET=arm-linux-gnueabihf-
CXX=$(TARGET)g++
CC=$(TARGET)gcc
CXXFLAGS=-std=c++17 -g -I$(IDIR) -I$(WIR_PI_DIR) 
LDFLAGS= -L$(WIR_PI_DIR) -lwiringPi $(WIRPI_LIBS) -L/usr/arm-linux-gnueabihf/include/mysql -lmysqlclient  
EXE=vixen2Pi

.PHONY: all clean wiringPi

#debug variable prints
##$(warning $(HEADERS))
##$(warning $(OBJ))
##$(warning $(SRC))
#$(warning $(LDFLAGS))

all: $(EXE)

$(EXE): $(OBJ) | wiringPi
ifeq ($(TARGET), arm-linux-gnueabihf-)
	ln -s ~/Projects/vixen2Pi/$(WIR_PI_DIR)/libwiringPi.so* ~/Projects/vixen2Pi/$(WIR_PI_DIR)/libwiringPi.so
	$(CXX) -o $@ $^ $(LDFLAGS)
	rm ~/Projects/vixen2Pi/$(WIR_PI_DIR)/libwiringPi.so
else
	$(CXX) -o $@ $^ $(LDFLAGS)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

wiringPi:
ifeq ($(TARGET), arm-linux-gnueabihf-)
		make -C $(WIR_PI_DIR) CC=$(CC) LIBS=$(WIRPI_LIBS)
else
		make -C $(WIR_PI_DIR)
endif

clean:
	make -C $(WIR_PI_DIR) clean
	rm $(OBJ_DIR)/*.o
	rm $(EXE)
