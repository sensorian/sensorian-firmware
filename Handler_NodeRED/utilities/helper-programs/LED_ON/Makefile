CXX = g++
CFLAGS = -Wall -Werror
CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

#CORE = GPIO
CORE = LED_ON
OBJS = main.o Utilities.o
FILES = Makefile main.c Utilities.c 

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


