/*
 * matrix.c:
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
#include <mcp23017.h>

#define ROW_OFFSET      100
#define COLUMN_BASE     108

unsigned char matrix [8][8] ;

PI_THREAD (matrixUpdater)
{
  int row, col ;
  unsigned char data ;

  piHiPri (50) ;

  for (;;)
  {
    for (row = 0 ; row < 8 ; ++row)
    {
      digitalWrite (ROW_OFFSET + row, 1) ; 
      for (col = 0 ; col < 8 ; ++col)
      {
        data = matrix [col][row] ;
        /**/ if (data == 0)
          continue ;
        else                    // Red
        {
          digitalWrite (COLUMN_BASE + col, 0) ; 
          delayMicroseconds (500) ;
          digitalWrite (COLUMN_BASE + col, 1) ;
        }
      }
      digitalWrite (ROW_OFFSET + row, 0) ;
    }
  }

  return NULL ;
}

void setupMatrix (void)
{
  int row, col ;

  wiringPiSetupSys () ;

  mcp23017Setup (100, 0x20) ;

  for (row = 0 ; row < 16 ; ++row)
  {
    pinMode (100 + row, OUTPUT) ;
    digitalWrite (100 + row, 0) ;
  }

}
char update [64] =
 " ****** "
 "*      *"
 "* *  * *"
 "*      *"
 "* *  * *"
 "*  **  *"
 "*      *"
 " ****** " ;

void matrixOff (void)
{
  int n ;
  int x, y ;
  char offMatrix [8][8] ;

  // setup matrix here
  for (x = 0 ; x < 8 ; ++x)
    for (y = 0 ; y < 8 ; ++y)
      if (update [x + 8 * y] == '*')
        offMatrix [x][y] = 0 ;
      else
        offMatrix [x][y] = 1 ;

  memcpy (matrix, offMatrix, sizeof (offMatrix)) ;
}

void matrixOn (void)
{
  int n ;
  int x, y ;
  char onMatrix [8][8] ;

  // setup matrix here
  for (x = 0 ; x < 8 ; ++x)
    for (y = 0 ; y < 8 ; ++y)
      if (update [x + 8 * y] == '*')
        onMatrix [x][y] = 1 ;
      else
        onMatrix [x][y] = 0 ;

  memcpy (matrix, onMatrix, sizeof (onMatrix)) ;
}

/*
 *********************************************************************************
 * The works
 *********************************************************************************
 */

int main (int argc, char *argv [])
{
  int x, y ;

  setupMatrix () ;
  piThreadCreate (matrixUpdater) ;

  delay (2000) ;

  for (;;)
  {
    matrixOn () ;
    delay (100) ;
    matrixOff ();
    delay (100) ;
  }

  return 0 ;
}
