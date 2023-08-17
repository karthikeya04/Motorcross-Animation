CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./
BUILD_DIR=bin
# BIN1=./bin/a2-model-rider 
# SRCS1=./src/rider.cpp ./src/gl_framework.cpp ./src/shader_util.cpp ./src/hierarchy_node.cpp ./src/texture.cpp
# INCLUDES1=./src/gl_framework.hpp ./src/shader_util.hpp ./src/rider.hpp ./src/hierarchy_node.hpp ./src/texture.hpp ./src/stb_image_write.h


BIN3=./bin/fmx-animate 
SRCS3=./src/track.cpp ./src/gl_framework.cpp ./src/shader_util.cpp ./src/hierarchy_node.cpp ./src/texture.cpp
INCLUDES3=./src/gl_framework.hpp ./src/shader_util.hpp ./src/track.hpp ./src/hierarchy_node.hpp ./src/texture.hpp ./src/stb_image_write.h

all: $(BIN3) $(BIN2)
# all: $(BIN1) $(BIN2) $(BIN3)

# $(BIN1): $(SRCS1) $(INCLUDES1)
# 	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)



$(BIN3): $(SRCS3) $(INCLUDES3)
	mkdir -p $(BUILD_DIR)
	g++ $(CPPFLAGS) $(SRCS3) -o $(BIN3) $(LDFLAGS) $(LIBS)

clean:
	rm -rf $(BUILD_DIR)
# clean:
# 	rm -f *~ *.o $(BIN1) $(BIN2) $(BIN3)