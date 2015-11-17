.PRECIOUS: %.o
TARGET=gpiolcd
OBJS=ipLcd.o gpioLcd.o commonLcd.o gpiomain.o
LIBS=-lwiringPi
ifdef SPI
TARGET=spilcd
OBJS=ipLcd.o spiLcd.o commonLcd.o spimain.o
LIBS=
endif
ifdef I2C
TARGET=i2clcd
LIBS=
OBJS=ipLcd.o i2cLcd.o commonLcd.o i2cmain.o
endif

#all:
#	echo $(TARGET) $(OBJS)
all : $(TARGET)

$(TARGET) : $(OBJS)
	gcc -Wall -O3 $(LIBS) -o $(TARGET) $(OBJS)

%.o : %.c
	gcc -Wall $^ -c -O3 $(LIBS) -o $@

clean:
	rm *lcd *.o
