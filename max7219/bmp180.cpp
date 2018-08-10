// (C) Copyright 2018 Dougie Lawson. All rights reserved.
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <fstream>
#include <ctime>
#include <csignal>
#include <cstdlib>
#include <cmath>
#include "sqlite3.h"

//#define DATABASE "/srv/bmp180/sensordata.db"
#define DATABASE "/home/pi_f/python/sensordata.db"
//#define DATABASE "/home/pi_f/c/sensordata.db"

#define PRESSURE "/sys/bus/iio/devices/iio:device0/in_pressure_input"
#define TEMPERATURE "/sys/bus/iio/devices/iio:device0/in_temp_input"

sqlite3 *db;
struct SensorData
{
	float temp;
	float pressure;
};

void sig_handler(int signum)
{
   std::cout << "Received signal " << signum << std::endl;
   sqlite3_close(db);
   std::exit(signum);
}

static int callback(void *data, int argc, char **argv, char **azColName){
   return 0;
}

SensorData readBMP180()
{
	SensorData readings;
        std::ifstream inputTemp(TEMPERATURE);
	int temp;
       	float pressure;
	inputTemp >> temp;
	readings.temp = floor((((float)temp / 1000.0) * 100) +0.5) / 100;
	std::ifstream inputPress(PRESSURE);
	inputPress >> pressure;
	std::cout << "In: " << pressure;
	readings.pressure = pressure * 10.0;
	std::cout << " Out: " << readings.pressure << std::endl;
	return readings;

}


void wait3min() 
{
	using std::chrono::system_clock;
	std::time_t tt = system_clock::to_time_t (system_clock::now());
	struct std::tm *ptm = std::localtime(&tt);
	std::cout << "Current time: " << std::put_time(ptm, "%X") << std::endl;
	int offset = 3 - (ptm->tm_min % 3);
	std::cout << "Waiting for " << offset << " minutes ..." << std::endl;
	ptm->tm_min+=offset; ptm->tm_sec=0;
	std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
	std::cout << std::put_time(ptm,"%X") << " reached!" << std::endl;
}

int main()
{
   std::signal(SIGTERM, sig_handler);

   int rc;
   char *zErrMsg = 0;
   const char* sqlini1 = "PRAGMA journal_mode=WAL";
   const char* sqlini2 = "VACUUM";
   const char* sql;

   const char* data = "Callback function called";
   rc = sqlite3_open(DATABASE, &db);
   if( rc )
   {
	std::cerr <<  "Can't open database: " <<  sqlite3_errmsg(db) << std::endl;
	std::exit(rc);
   }
   rc = sqlite3_exec(db, sqlini1, callback, (void*)data, &zErrMsg);
   if( rc )
   {
	std::cerr <<  "PRAGMA journal_mode: " <<  sqlite3_errmsg(db) << std::endl;
	std::exit(rc);
   }

   rc = sqlite3_exec(db, sqlini2, callback, (void*)data, &zErrMsg);
   if( rc )
   {
	std::cerr <<  "VACUUM: " <<  sqlite3_errmsg(db) << std::endl;
	std::exit(rc);
   }

   while (true)
   {
  	wait3min();
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK )
	{
	    std::cerr <<  "SQL BEGIN error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
	    std::exit(rc);
	}	

	SensorData values = readBMP180();
	std::cout << "Temp: " << values.temp << " Pressure: " << values.pressure << std::endl;
   
	sql = "insert into bmp_data (date_time, temp, pressure) values(datetime('now','localtime'), round(?,2), round(?,2));";

	sqlite3_stmt *stmt;
	rc = sqlite3_prepare(db, sql, -1, &stmt, 0);
	if( rc != SQLITE_OK )
	{
	    std::cerr <<  "SQL prepare error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
	    std::exit(rc);
	}	

	rc = sqlite3_bind_double(stmt, 1, values.temp);
	if( rc != SQLITE_OK )
	{
	    std::cerr <<  "SQL bind temp error " << std::endl;
	    std::exit(rc);
	}

	rc = sqlite3_bind_double(stmt, 2, values.pressure);
	if( rc != SQLITE_OK )
	{
	    std::cerr <<  "SQL bind pressure error: " << std::endl;
	    std::exit(rc);
	}	
	
	rc = sqlite3_step(stmt);
	if ( rc != SQLITE_DONE)
	{
	    std::cerr << "SQL step error " << std::endl;
	    std::exit(rc);
	}

	rc = sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK )
	{
	    std::cerr <<  "SQL END error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
	    std::exit(rc);
	}	

   }
   return 0;
}
