CXX = g++
CFLAGS = -Wall -Werror
CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = CAP1203
OBJS = main.o CAP1203.o i2c.o Utilities.o
FILES = Makefile CAP1203.h CAP1203.c main.c i2c.c i2c.h Utilities.c Utilities.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


