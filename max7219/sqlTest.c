// (C) Copyright 2016,2021, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#define __USE_XOPEN
#include <time.h>
#define ALTITUDE 122.5
#define DATABASE "/srv/bmp180/sensordata.db"

static int fetchResults(sqlite3_stmt* stmt)
{
	int i;
	const unsigned char* dateTime;
	double tempC;
	double tempF;
	double pressure;
	double pressureMSL;
	struct tm ltm = {0}; 
        // Date & time format is yyyy-mm-dd hh:mm:ss
	dateTime = sqlite3_column_text(stmt, 0);
	strptime(dateTime, "%Y-%m-%d %H:%M:%S", &ltm);
        printf("Date : %02d-%02d-%02d\n", ltm.tm_year-100, ltm.tm_mon+1, ltm.tm_mday );
	printf("Time : %02d:%02d:%02d\n", ltm.tm_hour, ltm.tm_min, ltm.tm_sec);
	tempC = sqlite3_column_double(stmt, 1);
        printf("°C   : %5s%2.1f\n", "     ", tempC );
        tempF = (9 * ( tempC / 5) + 32);
        printf("°F   : %5s%2.1f\n", "     ", tempF);
	pressure = sqlite3_column_double(stmt, 2);
        printf("QFE  : %2s%4.2f\n", "  ", pressure);
        // Database holds pressure at local altitude
        // Correct pressure for mean sea level
        pressureMSL = pressure / powf(1 - ( ALTITUDE / 44330.0) , 5.255);
        printf("QNH  : %2s%4.2f\n", "  ", pressureMSL);
	return 0;
}

int main(int argc, char* argv[])
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	int rc;
	char* sql;
	char* zErrMsg = 0;
	rc = sqlite3_open(DATABASE, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	rc = sqlite3_exec(db, "PRAGMA query_only", NULL, NULL, &zErrMsg);
	if( rc )
	{
		fprintf(stderr, "query-only failed %s\n" ,  sqlite3_errmsg(db));
		exit(rc);
	}
	sql = "SELECT datetime(a.date_time, \'localtime\') as date_time, a.temp, a.pressure from bmp_data a where a.date_time in (select max(b.date_time) from bmp_data b);";
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
			fprintf(stderr, "sqlite_step failed (%d) %s\n", s, sqlite3_errmsg(db));
			exit(s);
		}
	}
	sqlite3_clear_bindings(stmt);
	sqlite3_reset(stmt);
	sqlite3_close(db);
	return 0;
}
