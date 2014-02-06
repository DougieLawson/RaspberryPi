#define ORIENT180
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

#include <wiringPi.h>
#include <mcp23017.h>
#define wPi wiringPiI2CWriteReg8

int mcp;

const char byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    printf("%s", b);
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

void writeToLED(int rowPins, int colPins)
{
  printf("rowPins: ");
  byte_to_binary(rowPins);
  printf(" colPins: ");
  byte_to_binary(colPins);
  printf(" \r\n");
  wPi(mcp, PORTA, 0x00); // all off 
  delayMicroseconds (500) ;
  wPi(mcp, PORTB, rowPins); 
  delayMicroseconds (500) ;
  wPi(mcp, PORTA, colPins); 
  delay(200);
}

void setPattern(int rowPattern, int colPattern) 
{
//   writeToLED(~rowPattern, reverseBits(colPattern)); 
//   writeToLED(~colPattern, rowPattern); 
//   writeToLED(~reverseBits(rowPattern),colPattern); 
//   writeToLED(~reverseBits(colPattern),reverseBits(rowPattern));
   writeToLED(~rowPattern, colPattern);
}

char matrix [8] ;

PI_THREAD (matrixUpdater)
{
  int row, col ;
  unsigned char data ;

  piHiPri (50) ;

  for (;;)
  {
    for (row = 0 ; row < 8 ; ++row)
    {
      printf("%d ",row);
      setPattern(0x80 >> row, matrix[row]);
    }
  }

  return NULL ;
}

char updByte [8] = { 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0};

void matrixOff (void)
{
  int n ;
  int x, y ;
  char offMatrix [8];

  // setup matrix here
  for (x = 0 ; x < 8 ; ++x)
    offMatrix [x] = 0x00 ;

  memcpy (matrix, offMatrix, sizeof (offMatrix)) ;
}

void matrixOn (void)
{
  int n ;
  int x, y ;
  char onMatrix [8];

  for (x = 0 ; x < 8 ; ++x)
    onMatrix [x] = updByte[x] ;

  memcpy (matrix, onMatrix, sizeof (onMatrix)) ;
}

int setupMatrix (void)
{
  int row, col ;

  wiringPiSetupSys () ;

  int mcp = wiringPiI2CSetup(0x20);
  wPi(mcp, DIRA, 0x00);
  wPi(mcp, DIRB, 0x00);
  matrixOff();
  return mcp;
}

int main (int argc, char *argv [])
{
  int x, y, z;
  mcp = setupMatrix () ;
  piThreadCreate (matrixUpdater) ;

  delay (500) ;
  for (;;)
  {
      matrixOn () ;
      delay (100) ;
      matrixOff ();
      delay (100) ;
  }

  return 0 ;
}
