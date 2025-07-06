CXX       = g++ -std=c++23
CC        = gcc
CXXFLAGS += -Wall
CFLAGS   += -Wall
INCLUDES  = -I. -I./include -I./glad/include
LDFLAGS   = -ldl -lGL -lglfw

CPP_SOURCES = $(wildcard *.cpp)
C_SOURCES   = ./glad/src/glad.c
OBJECTS     = $(CPP_SOURCES:.cpp=.o) $(C_SOURCES:.c=.o)
TARGET      = main

.PHONY: all clean cleanall

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o ./glad/src/*.o *~

cleanall: clean
	rm -f $(TARGET)

