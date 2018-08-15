// (C) Copyright 2016, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include "max7219.h"
#include "spiLED.h"
#define ALTITUDE 112.2
#define DATABASE "/srv/bmp180/sensordata.db"
//#define DATABASE "/home/pi_f/python/sensordata.db"

max7219 header;
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   char chars[30];
   char chars1[9];
   char *token1;
   char *token2;
   char *stringp = chars;
   float tempF;
   float pressureMSL;
   const char *delim = " ";
   
   for(i=0; i<argc; i++){
      if (strcmp(azColName[i], "date_time") == 0) 
      {
        // Date & time format is yyyy-mm-dd hh:mm:ss
        // Split that into 
        // Date (token1): yy-mm-dd
        // Time (token2): hh:mm:ss
        sprintf(chars, "%s", argv[i]);
        token1 = strsep(&stringp, delim);
        token2 = strsep(&stringp, delim);
        
        // Ugly use of memcpy to strip off century digits
        memcpy(chars1, &token1[2], 8);
        chars1[8] = '\0';
        printf("Date       :** %s **\r\n", chars1);
        printf("Time       :** %s **\r\n", token2);
      }
      else if (strcmp(azColName[i], "temp") == 0)
      {
        sprintf(chars,"%8s",argv[i]);
        tempF = (9 * ( atof(chars) / 5) + 32);
        printf("Temperature:** %s **\r\n", chars);
        printf("Temp (F)   :** %2.1f **\r\n", tempF);
      }
      else if (strcmp(azColName[i], "pressure") == 0)
      {
        sprintf(chars,"%8s",argv[i]);
        // Database holds pressure at local altitude
        // Correct pressure for mean sea level
        pressureMSL = atof(chars) / powf(1 - ( ALTITUDE / 44330.0) , 5.255);
        printf("Pressure   :** %4.2f **\r\n", atof(chars));
        printf("PressureMSL:** %4.2f **\r\n", pressureMSL);
      }
      else 
      {
        printf("Unknown column: %s\r\n", azColName[i]);
      }
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   rc = sqlite3_open(DATABASE, &db);
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
   return 0;
}
