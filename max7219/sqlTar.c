// (C) Copyright 2020,2021, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sqlite3.h> 
#include "max7219.h"
#include "spiLED.h"
#include "getTariff.h"

#define DATABASE "/home/pi_d/octopus/tariffs.db"

max7219 header;

void fetchResults(sqlite3_stmt* stmt)
{
	double octPrice;
	char price[12];
	octPrice = sqlite3_column_double(stmt, 0);
	sprintf(price, "%.3f P", octPrice);
	clearDisplay(&header);
	writeDigits(&header, price);
}

void getTariff(void)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	char *zErrMsg = 0;
	int rc;
	char* sql;
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
	sql = "select price from agile_tariffs where julianday('now') >= julianday(period_start) and julianday('now') <= julianday(period_end);";
	sqlite3_prepare_v3(db, sql, strlen(sql)+1, 0, &stmt, NULL);
	while (1)
	{
		int s;
		s = sqlite3_step(stmt);
		if (s == SQLITE_ROW) { fetchResults(stmt);
		}
		else if (s == SQLITE_DONE)
		{
			break;
		}
		else
		{
			fprintf(stderr, "getTarrif: sqlite_step failed (%d) %s\n", s, sqlite3_errmsg(db));
			exit(s);
		}
	}
	sqlite3_clear_bindings(stmt);
	sqlite3_reset(stmt);
	sqlite3_close(db);
}
