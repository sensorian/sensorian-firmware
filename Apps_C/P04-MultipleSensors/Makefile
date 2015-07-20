CXX = g++
CFLAGS = -Wall 
CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = MultiSensor
OBJS = main.o APDS9300.o i2c.o MPL3115A2.o Utilities.o
FILES = Makefile APDS9300.h APDS9300.c MPL3115A2.c MPL3115A2.h main.c i2c.c i2c.h Utilities.c Utilities.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


