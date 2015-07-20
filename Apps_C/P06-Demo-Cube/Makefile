CXX = g++
CFLAGS = -Wall 
CFLAGS += -g
#CFLAGS += -O3
LIBS    = -lbcm2835

CORE = Cube
OBJS = main.o TFT.o SPI.o GFX.o Cube.o Transforms.o
FILES = Makefile SPI.h SPI.c main.c TFT.c TFT.h Font.h GFX.c GFX.h Cube.c Cube.h Transforms.c Transforms.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


