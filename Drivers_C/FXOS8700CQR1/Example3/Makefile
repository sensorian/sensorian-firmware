CXX = g++
CFLAGS = -Wall 
CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = FXOS8700CQ
OBJS = main.o FXOS8700CQ.o i2c.o Utilities.o
FILES = Makefile FXOS8700CQ.c FXOS8700CQ.h main.c i2c.c i2c.h MemoryMap.h Utilities.c Utilities.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


