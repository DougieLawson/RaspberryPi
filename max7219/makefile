.PRECIOUS: %.o
TARGET=LEDspi
OBJS=sqlF.o spiLED.o
LIBS=-lsqlite3
OPT=

#all:
#	echo $(TARGET) $(OBJS)
all : $(TARGET)

$(TARGET) : $(OBJS)
	gcc -Wall -O3 $(OPT)  $(LIBS) -o $(TARGET) $(OBJS)

%.o : %.c
	gcc -Wall $^ -c -O3 $(OPT) $(LIBS) -o $@

clean:
	rm LEDspi *.o
