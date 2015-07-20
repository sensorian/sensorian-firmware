CXX = gcc
CFLAGS = -Wall 	
#-Werror
CFLAGS += -g
#CFLAGS += -O3
#LIBS    = -lbcm2835

CORE = Serial
OBJS = main.o Serial.o
FILES = Makefile Serial.h Serial.c main.c 

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


