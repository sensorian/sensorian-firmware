CXX = gcc
CFLAGS = -Wall -std=c99
#CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = libFXO.so
OBJS = FXOS8700CQ.o i2c.o
FILES = Makefile FXOS8700CQ.c FXOS8700CQ.h i2c.c i2c.h MemoryMap.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -shared -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c -Werror -fPIC $(CFLAGS) $< -o $@


