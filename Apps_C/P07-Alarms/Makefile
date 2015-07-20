CXX = gcc
CFLAGS = -Wall -Werror
CFLAGS += -g
CFLAGS += -std=c99
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = MCP79410
OBJS = main.o MCP79410.o MPL3115A2.o i2c.o Utilities.o
FILES = Makefile MCP79410.h MCP79410.c MPL3115A2.c MPL3115A2.h Utilities.c Utilities.h main.c i2c.c i2c.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


