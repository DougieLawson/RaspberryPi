#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include "max7219.h"
#include "spiLED.h"
#define ALTITUDE 112.2 // FIXME change to local altitude 

max7219 header;
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   char chars[30];
   char chars1[9];
   char *token1;
   char *token2;
   char *stringp = chars;
   const char *delim = " ";
   float tempF;
   float pressureMSL;
   
   for(i=0; i<argc; i++){
      if (strcmp(azColName[i], "date_time") == 0) 
      {
        clearDisplay(&header);
        sprintf(chars, "%s", argv[i]);
        token1 = strsep(&stringp, delim);
        token2 = strsep(&stringp, delim);
        
        memcpy(chars1, &token1[2], 8);
        chars1[8] = '\0';
        writeDigits(&header, chars1);
        sleep(4);

        writeDigits(&header, token2);
        sleep(4);
      }
      else if (strcmp(azColName[i], "temp") == 0)
      {
        clearDisplay(&header);
        sprintf(chars,"%8s",argv[i]);
        writeDigits(&header, chars);
        sleep(8);
        clearDisplay(&header);
        tempF = (9 * ( atof(chars) / 5) + 32);
        sprintf(chars,"%2.1f", tempF);
        writeDigits(&header, chars);
        sleep(4);
      }
      else if (strcmp(azColName[i], "pressure") == 0)
      {
        clearDisplay(&header);
        sprintf(chars,"%8s",argv[i]);
        writeDigits(&header, chars);
        sleep(4);
        clearDisplay(&header);
        pressureMSL = atof(chars) / powf(1 - ( ALTITUDE / 44330.0) , 5.255);
        sprintf(chars,"%4.2f", pressureMSL);
        writeDigits(&header, chars);
        sleep(4);
      }
      else 
      {
        printf("Unknown column: %s\r\n", azColName[i]);
      }
   }
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   FILE *fp;
   int rc;
   int bright;
   char lightordark[40];
   char *zErrMsg = 0;
   char *sql;
   char *LoDstringp = lightordark;
   char *LoDtoken;

   const char filename[21] = "/var/run/lightordark";
   const char *delim = " ";
   const char* data = "Callback function called";

   initialiseDisplay(&header);
   clearDisplay(&header);

   while (1==1)
   {
   /* Open database */
     rc = sqlite3_open("/srv/bmp180/sensordata.db", &db);
     if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
     }

     /* Create SQL statement */
     sql = "SELECT a.date_time, a.temp, a.pressure from bmp_data a where a.date_time in (select max(b.date_time) from bmp_data b);";

     /* Execute SQL statement */
     rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
     if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
     }
     sqlite3_close(db);


     /* Set the display brightness at dawn / dusk */

     /* FIXME  Optional code to read /var/run/lightordark and 
        alter the MAX7219 brighness register.

        I have a script running sunwait that writes to /var/run/lightordark
        at dawn and dusk */

/* FIXME

     fp = fopen(filename,"r");
     fgets(lightordark, sizeof lightordark, fp); 
     LoDtoken = strsep(&LoDstringp, delim);

     if (strcmp(LoDtoken, "dark") == 0) bright = 10;
     else bright = 3;

     setBrightness(&header, bright);
     fclose(fp);
     LoDstringp = lightordark;
FIXME */

   }
   return 0;
}
