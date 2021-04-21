// (C) Copyright 2016-2021, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <signal.h>
#define __USE_XOPEN
#include <time.h>
#include "max7219.h"
#include "spiLED.h"
#include "getTariff.h"
#define ALTITUDE 122.5
#define DATABASE "/srv/bmp180/sensordata.db"

max7219 header;
struct sigaction act;

void sig_handler(int signum, siginfo_t *info, void *ptr)
{
	clearDisplay(&header);
	printf("Received signal %d\n", signum);
	printf("Signal originated from process %lu\n", (unsigned long)info->si_pid);
	exit(0);
}

void readBrightness()
{
		/* Set the display brightness at dawn / dusk */
	FILE *fp;
	const char filename[21] = "/var/run/lightordark";
	const char *delim = " ";
	int bright;
	char lightordark[40];
	char charst[12];
	char *LoDstringp = lightordark;
	char *LoDtoken;
	char *LoDtime;
	fp = fopen(filename,"r");
	fgets(lightordark, sizeof lightordark, fp);
	LoDtoken = strsep(&LoDstringp, delim);
	LoDtime = strsep(&LoDstringp, delim); /* skip @ */
	LoDtime = strsep(&LoDstringp, delim); /* got Time */
	sprintf(charst," %8s ", LoDtime);
	writeDigits(&header, charst);
	sleep(4);
	if (strcmp(LoDtoken, "dark") == 0) bright = 7;
	else bright = 3;
	setBrightness(&header, bright); fclose(fp);
	LoDstringp = lightordark;
}

static int fetchResults(sqlite3_stmt* stmt)
{
	int i;
	char chars[12];
	const unsigned char* dateTime;
	double tempC;
	double tempF;
	double pressure;
	double pressureMSL;
	struct tm ltm = {0}; 
        // Date & time format is yyyy-mm-dd hh:mm:ss
	dateTime = sqlite3_column_text(stmt, 0);
	strptime(dateTime, "%Y-%m-%d %H:%M:%S", &ltm);
        clearDisplay(&header);
        sprintf(chars, "%02d-%02d-%02d", ltm.tm_year-100, ltm.tm_mon+1, ltm.tm_mday );
        writeDigits(&header, chars);
        sleep(4);
        clearDisplay(&header);
	sprintf(chars, " %02d:%02d:%02d ", ltm.tm_hour, ltm.tm_min, ltm.tm_sec);
        writeDigits(&header, chars);
        sleep(4);
        clearDisplay(&header);
	tempC = sqlite3_column_double(stmt, 1);
        sprintf(chars,"%5s%2.1f", "     ", tempC );
        writeDigits(&header, chars);
        sleep(8);
        clearDisplay(&header);
        tempF = (9 * ( tempC / 5) + 32);
        sprintf(chars,"%5s%2.1f", "     ", tempF);
        writeDigits(&header, chars);
        sleep(4);
        clearDisplay(&header);
	pressure = sqlite3_column_double(stmt, 2);
        sprintf(chars,"%2s%4.2f", "  ", pressure);
        writeDigits(&header, chars);
        sleep(4);
        clearDisplay(&header);
        // Database holds pressure at local altitude
        // Correct pressure for mean sea level
        pressureMSL = pressure / powf(1 - ( ALTITUDE / 44330.0) , 5.255);
        sprintf(chars,"%2s%4.2f", "  ", pressureMSL);
        writeDigits(&header, chars);
        sleep(4);
	return 0;
}

int main(int argc, char* argv[])
{
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = sig_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &act, NULL);
	sqlite3 *db;
	sqlite3_stmt* stmt;
	int rc;
	char *zErrMsg = 0;
	char *sql;
	initialiseDisplay(&header);
	clearDisplay(&header);
	while (1)
	{
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
		sql = "SELECT datetime(a.date_time,'localtime') as tstamp, a.temp, a.pressure from bmp_data a where a.date_time in (select max(b.date_time) from bmp_data b);";
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
		readBrightness();
		getTariff();
		sleep(4);
	}
	return 0;
}
