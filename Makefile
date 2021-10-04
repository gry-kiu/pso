CC = gcc
CFLAGS = -Wall -Werror
TARGET1 = pso
TARGET2 = pso.so
OBJECTS = pso.o

all : $(TARGET1) $(TARGET2)

$(TARGET1) : $(TARGET2)
	$(CC) $(CFLAGS) -o $@ demo.c $^ -lm -Wl,-rpath=$(shell pwd)

$(TARGET2) : $(OBJECTS)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^

clean :
	rm -f *.o $(TARGET1) $(TARGET2)
