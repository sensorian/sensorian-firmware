CXX = gcc
CFLAGS = -Wall -std=c99
#CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = libCAP.so
OBJS = CAP1203.o i2c.o
FILES = Makefile CAP1203.c CAP1203.h i2c.c i2c.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -shared -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c -Werror -fPIC $(CFLAGS) $< -o $@


