ifeq ($(shell uname), Linux)
	OPTS=$(shell /usr/bin/sdl2-config --cflags) -I. -I.. -I -Wall
	LIBS=$(shell /usr/bin/sdl2-config --libs)
	LIBS += -lGL -lGLU -lGLEW -lX11
endif

ifeq ($(shell uname), Darwin)
	OPTS=$(shell /usr/local/bin/sdl2-config --cflags) -I. -I.. -Wall -g
	LIBS=$(shell /usr/local/bin/sdl2-config --libs)
	LIBS += -framework OpenGL -lGLEW -ldl
endif

all : main

main : main.o
	c++ $(OPTS) -o $@ $^ $(LIBS)

%.o : %.cpp
	c++ $(OPTS) -o $@ -c $<

%.o : %.c
	cc  $(OPTS) -o $@ -c $<

clean :
	rm -f *.o
	rm -f *.a
	rm -f *.out
	rm -f particle
