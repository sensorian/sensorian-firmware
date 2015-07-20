CXX = g++
CFLAGS = -Wall 
CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = IP
OBJS = main.o TFT.o SPI.o GFX.o Utilities.o
FILES = Makefile SPI.h SPI.c main.c TFT.c TFT.h Font.h GFX.c GFX.h Utilities.c Utilities.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


