CC=g++
CFLAGS=-c -Wall -std=c++20
LFLAGS=
SRCS=$(wildcard ./source/*.cpp)
OBJS=$(patsubst ./source/%.cpp, ./object/%.o, $(SRCS))
EXECUTABLE=./bin/program

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

./object/%.o: ./source/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ./object/*.o $(EXECUTABLE)
