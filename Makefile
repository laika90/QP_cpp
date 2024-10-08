.PHONY: all clean re run rerun

CC     = g++
CFLAGS = -Wall
TARGET = build/main
SRCS   = src/main.cpp 
OBJS   = $(SRCS:src/%.cpp=build/%.o)
INCDIR = -Iinclude
LIBDIR =
LIBS   =

$(shell mkdir -p build)

all: $(TARGET)

release: OPTFLAGS = -O3
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCDIR) -o $@ $^ $(LIBDIR) $(LIBS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCDIR) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)$(EXE)

re: clean all

run:
	./build/main

rerun: re run