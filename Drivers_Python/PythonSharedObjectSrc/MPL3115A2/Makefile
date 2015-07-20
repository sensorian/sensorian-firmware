CXX = gcc
CFLAGS = -Wall -std=c99
#CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = libMPL.so
OBJS = MPL3115A2.o i2c.o
FILES = Makefile MPL3115A2.c MPL3115A2.h i2c.c i2c.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -shared -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c -Werror -fPIC $(CFLAGS) $< -o $@


