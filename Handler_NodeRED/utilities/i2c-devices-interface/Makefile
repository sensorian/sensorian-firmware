CXX = gcc
CFLAGS = -Wall -std=c99
#CFLAGS += -g -shared -fPIC
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = libsensorian.so
OBJS = SensorsInterface.o MPL3115A2.o i2c.o APDS9300.o CAP1203.o FXOS8700CQ.o MCP79410.o led.o Utilities.o
FILES = Makefile MPL3115A2.h MPL3115A2.c APDS9300.c APDS9300.h CAP1203.c CAP1203.h FXOS8700CQ.c FXOS8700CQ.h MCP79410.c MCP79410.h led.c led.h SensorsInterface.c i2c.c i2c.h Utilities.c Utilities.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -shared -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c -Werror -fPIC $(CFLAGS) $< -o $@


