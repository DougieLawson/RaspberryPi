.PRECIOUS: %.o
TARGET=LEDspi
OBJS=sqlF.o spiLED.o
LIBS=-lsqlite3

#all:
#	echo $(TARGET) $(OBJS)
all : $(TARGET)

$(TARGET) : $(OBJS)
	gcc -Wall -O3 $(LIBS) -o $(TARGET) $(OBJS)

%.o : %.c
	gcc -Wall $^ -c -O3 $(LIBS) -o $@

clean:
	rm LEDspi *.o
