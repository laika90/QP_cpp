.PHONY: all clean re run rerun

CC     = g++
CFLAGS = -Wall
TARGET = build/main
SRCS   = src/main.cpp 
OBJS   = $(SRCS:src/%.cpp=build/%.o)
INCDIR = -Iinclude
LIBDIR =
LIBS   =


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCDIR) -o $@ $^ $(LIBDIR) $(LIBS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)$(EXE)

re: clean all

run:
	./build/main

rerun: re run