#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <sqlite3.h>
#include "mqttMessage.hpp"

using namespace std;

class DBLite {

    private:
	    sqlite3_stmt* stmt = NULL;
	    sqlite3 *db;
	    char *zErrMsg;
	    int rc;
	    void fetchResults()
	    {
		    int hourValue;
		    double temp;
		    const unsigned char* dT;
		    string hour;
		    stringstream mqttPayLoadStr;
		    stringstream sOutput;
		    string mqttPayLoad;
		    int rowNum;

		    while(1) {
			    int s;
			    s = sqlite3_step(stmt);
			    if (s == SQLITE_ROW)
			    {
				    rowNum = sqlite3_column_int(stmt, 0);
				    dT = sqlite3_column_text(stmt, 1);
				    temp = sqlite3_column_double(stmt, 2);

				    string dateTime((char*)dT, 19);
				    if (dateTime.compare(13, 4, ":00:") == 0)
				    {
					    hour = dateTime.substr(11,2);
				            mqttMessage tempMQTT("tempMQTT", "dbread/out/temp", "10.1.1.11", 1883);
					    mqttPayLoadStr << (rowNum+1) / 20;
					    mqttPayLoadStr << ":" << hour;
					    mqttPayLoadStr << ":" << temp << endl;
					    mqttPayLoad = mqttPayLoadStr.str();

				            tempMQTT.send_message(mqttPayLoad.c_str());
					    /*
				            sOutput << "{\"rownum\":" << (rowNum+1) / 20;
				            sOutput << ", \"hour\": " << hour;
				            sOutput << ", \"temp\": " << temp << "}," << endl;
				            cout << sOutput.str();
				            sOutput.str(std::string());
				            sOutput.clear();
					    */
					    mqttPayLoadStr.str(std::string());
					    mqttPayLoadStr.clear();
				    }
			    }
			    else if (s == SQLITE_DONE)
			    {
				    return;
			    }
			    else
			    {
				    cerr << "sqlite3_step failed " << endl;
				    exit(1);
			    }
		    }
	    }

	    void checkDBErrors()
	    {
		   //cout << "check DB errors" << endl;
		   if( rc )
		   {
			    cout << "DB Error: " << sqlite3_errmsg(db) << endl;
			    closeDB();
		    }
	    }
	    void initDB()
	    {
		   //cout << "init DB " << endl;
		   rc = sqlite3_open("/srv/bmp180/sensordata.db", &db);
		   checkDBErrors();
	    }
    public:
	    DBLite()
	    {
		   //cout << "DBLite() " << endl;
		   initDB();
	    }
	    void showTable()
	    {
		   //cout << "showTable() " << endl;

	    	   
		   string sql = "SELECT\
		   row_number() over(order by date_time) rownum,\
		   date_time, temp FROM bmp_data\
		   WHERE date_time BETWEEN datetime('now','-1440 minutes')\
		   AND datetime('now') ;";
		   sqlite3_prepare_v3(db, sql.c_str(), sql.length() +1, 0, &stmt, NULL);
		   fetchResults();
		   //            rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	    }
	    void closeDB()
	    {
		   //cout << "closeDB() " << endl;
		   sqlite3_close(db);
	    }
};

