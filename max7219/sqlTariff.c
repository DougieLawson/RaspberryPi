// (C) Copyright 2016, 2021, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#define __USE_XOPEN
#include <time.h>
#include <math.h>
#define DATABASE "/home/pi_d/octopus/tariffs.db"

static int fetchResults(sqlite3_stmt* stmt)
{
   int i;
   	const unsigned char* dateTimeS;
   	const unsigned char* dateTimeE;
	double price;
   
       	struct tm ltmS = {0}; 
       	struct tm ltmE = {0}; 
        // Date & time format is yyyy-mm-dd hh:mm:ss
	dateTimeS = sqlite3_column_text(stmt, 0);
	dateTimeE = sqlite3_column_text(stmt, 1);
	price = sqlite3_column_double(stmt, 2);
	strptime(dateTimeS, "%Y-%m-%d %H:%M:%S", &ltmS);
	strptime(dateTimeE, "%Y-%m-%d %H:%M:%S", &ltmE);

	printf("Start \n");
        printf("Date       :** %04d-%02d-%02d **\r\n", ltmS.tm_year+1900, ltmS.tm_mon+1, ltmS.tm_mday);
        printf("Time       :** %02d:%02d:%02d **\r\n", ltmS.tm_hour, ltmS.tm_min, ltmS.tm_sec);
        
	printf(" End \n");
        printf("Date       :** %04d-%02d-%02d **\r\n", ltmE.tm_year+1900, ltmE.tm_mon+1, ltmE.tm_mday);
        printf("Time       :** %02d:%02d:%02d **\r\n", ltmE.tm_hour, ltmE.tm_min, ltmE.tm_sec);
	printf("Price      :** %.3f P **\r\n", price);
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	char* sql;
	rc = sqlite3_open(DATABASE, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	sql = "select period_start, period_end, price from agile_tariffs where julianday('now') >= julianday(period_start) and julianday('now') <= julianday(period_end);";
	sqlite3_prepare_v3(db, sql, strlen(sql)+1, 0, &stmt, NULL);
	while (1)
	{
		int s;
		s = sqlite3_step(stmt);
		if (s == SQLITE_ROW)
		{
			fetchResults(stmt);
		}
		else if (s == SQLITE_DONE)
		{
		       	break;
		}
		else
		{
			fprintf(stderr, "sqlite_step failed (%d) %s\n", s, sqlite3_errmsg(db)); exit(s);
		}
	}
	sqlite3_clear_bindings(stmt);
	sqlite3_reset(stmt);
	sqlite3_close(db);
	return 0;
}
