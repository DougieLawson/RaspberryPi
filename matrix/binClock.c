#define ADDR   0x20
#define DIRA   0x00
#define DIRB   0x01
#define PORTA  0x12
#define PORTB  0x13

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <wiringPi.h>
#include <mcp23017.h>
#define wPi wiringPiI2CWriteReg8

int mcp;

const char byte_to_binary(int ln, int x) {
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "X" : " ");
    }
    printf("%d: %s:\n",ln, b);
}

void writeToLED(int rowPins, int colPins) {
  wPi(mcp, PORTA, 0x00); // all off 
  delayMicroseconds(50);
  wPi(mcp, PORTB, rowPins); 
  delayMicroseconds(50);
  wPi(mcp, PORTA, colPins); 
}

int reverseBits(char myByte) 
{
  int bit;
  int value = 0;

  for (bit = 0; bit < 8; bit++)
  {
    if ((myByte & (1 << bit)))
    {
      value |= (0x80 >> bit);
    }
  }
  return value;
}


void setPattern(int rowPattern, int colPattern) {
   writeToLED(~colPattern, reverseBits(rowPattern));
   }

char matrix[8];

PI_THREAD(matrixUpdater) {
  int row, col;
  unsigned char data;

  piHiPri(50);

  for(;;) {
    for(row = 0; row < 8; ++row) {
      setPattern(0x80 >> row, matrix[row]);
    }
  }

  return NULL;
}

void TimeMatrix(void) { 
  
  int i;
  time_t current_time;
  struct tm * time_info;
  char timeString[9];  // space for "HH:MM:SS\0"
  char timeMatrix[8];

  time(&current_time);
  time_info = localtime(&current_time);

  strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
//  printf("%s\n", timeString);
  for(i = 0; i < 8; i++) { 
    timeMatrix[i] = ( timeString[i] - '0');
    if (timeMatrix[i] > 0x09) {
      timeMatrix[i] = 0x00;
    } else {
      timeMatrix[i] = timeMatrix[i] * 4;
    }
//    byte_to_binary(i, timeMatrix[i]);
  }
  memcpy(matrix, timeMatrix, sizeof (timeMatrix));
}

void matrixOff(void) {
  int x, y;
  char offMatrix[8];

  for(x = 0; x < 8; ++x)
    offMatrix[x] = 0x00;

  memcpy(matrix, offMatrix, sizeof (offMatrix));
}

int setupMatrix (void) {
  wiringPiSetupSys () ;
  int mcp = wiringPiI2CSetup(0x20);
  wPi(mcp, DIRA, 0x00);
  wPi(mcp, DIRB, 0x00);
  matrixOff();
  return mcp;
}


int main (int argc, char **argv) {

  mcp = setupMatrix();
  piThreadCreate(matrixUpdater);

  for (;;) {
    TimeMatrix();
    delay(1000);
  }

  return 0;
}

