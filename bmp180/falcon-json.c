// (C) Copyright 2016, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#define ALTITUDE 122.5
#define DATABASE "/srv/bmp180/sensordata.db"
//#define DATABASE "/home/pi_f/python/sensordata.db"

char *comma = " ";
int main(int argc, char* argv[])
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char* sql;
	char* sqlP;
	const unsigned char* date_time;
	char date_time_out[12];
	double temp;
	double pressure;
	double pressureMSL;
	sqlite3_stmt* stmt;
	printf("Content-Type: application/json\r\n\r\n{");
	printf("\"bmp_data\":[\n");
	setenv("TZ", "Europe/London", 1);

	rc = sqlite3_open(DATABASE, &db);
	if( rc )
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	sqlP = "PRAGMA query-only";
	rc = sqlite3_exec(db, sqlP, NULL, NULL, &zErrMsg);

	sql = "SELECT datetime(date_time,'localtime') as date_time, temp, pressure FROM bmp_data where date_time between datetime('now', '-1 day') and datetime('now');";

	sqlite3_prepare_v3(db, sql, strlen(sql) +1, 0, &stmt, NULL);
	while (1) {
		int s;
		s = sqlite3_step(stmt);
		if (s == SQLITE_ROW) 
		{
			printf("%s{", comma);
			date_time = sqlite3_column_text(stmt, 0);
			strncpy(date_time_out, date_time+5, 11); 
			printf("\"date_time\":\"%s\"", date_time_out);
			temp = sqlite3_column_double(stmt, 1);
			printf(",\"temp\":%.1f", temp);
			pressure = sqlite3_column_double(stmt, 2);
			pressureMSL = pressure / powf(1 - ( ALTITUDE / 44330.0) , 5.255);
			printf(",\"QFE\":%4.2f", pressure);
			printf(",\"QNH\":%4.2f", pressureMSL);
			comma=",";
			printf("}\n");
	       	}
		else if (s == SQLITE_DONE)
		{
			break;
		}
		else
		{
			fprintf(stderr, "Sqlite3_step failed\n");
			exit(1);
		}
	
	}
	printf("]}\n");
	sqlite3_clear_bindings(stmt);
	sqlite3_reset(stmt);
	sqlite3_close(db);
	return 0;
}
