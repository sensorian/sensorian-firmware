CXX = g++
CFLAGS = -Wall 
CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = CAPTOUCH
OBJS = main.o TFT.o SPI.o GFX.o i2c.o CAP1203.o Utilities.o
FILES = Makefile SPI.h SPI.c main.c TFT.c TFT.h Font.h GFX.c GFX.h CAP1203.h CAP1203.c i2c.c i2c.h Utilities.c Utilities.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


