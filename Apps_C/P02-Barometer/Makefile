CXX = g++
CFLAGS = -Wall 
CFLAGS += -g
LIBS    = -lbcm2835

CORE = WeatherStation
OBJS = main.o MPL3115A2.o i2c.o SPI.o TFT.o GFX.o Utilities.o
FILES = Makefile MPL3115A2.h MPL3115A2.c main.c i2c.c i2c.h SPI.c SPI.h GFX.c GFX.h \
Font.h TFT.c TFT.h Altimeter.h Barometer.h Thermometer.h Utilities.c Utilities.h

all: $(CORE)

$(CORE): $(OBJS) $(FILES)
	$(CXX) $(CFLAGS) -o $(CORE) $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


