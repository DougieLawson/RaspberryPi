#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <sqlite3.h>
#include "mqttMessage.hpp"

using namespace std;

class DBLite {

    private:
        sqlite3 *db;
        char *zErrMsg;
        int rc;
        char* sql;
        sqlite3_stmt *stmt;

        static int callback(void* NotUsed, int argc, char **argv, char **azColName)
        {
            int hour = 0;
            stringstream sOutput;
            stringstream sColName;
            stringstream sColValue;
	    stringstream sRowNum;
            string colName;
            string tempValue;
            string dateTimeValue;
            string hourValue;
	    string rowValue;
	    int rowNumValue;
	    int rowNum;
            for(int i = 0; i < argc; i++)
            {
                sColName << azColName[i];
                sColValue.str(argv[i]);
                sColName >> colName;

                if (colName.compare("rownum") == 0)
                {
                    sColValue >> rowNumValue;
		    rowNum = int((rowNumValue+1)/20);
		    sRowNum << rowNum;
		    rowValue = sRowNum.str();
                }
                if (colName.compare("date_time") == 0)
                {
                    sColValue >> tempValue;
                    sColValue >> dateTimeValue;
                    if (dateTimeValue.compare(2, 4, ":00:") == 0)
                    {
                        hour = 1;
                        hourValue = dateTimeValue.substr(0,2);
                    }
                }

                if (colName.compare("temp") == 0 && hour == 1)
                {
                    sColValue >> tempValue;
                    string sOutputStr;
		    sOutput << "{\"rownum\":" << rowValue;
                    sOutput << ", \"hour\": " << hourValue;
		    sOutput << ", \"temp\": " << tempValue << "}," << endl;
                    hour = 0;
                    sOutputStr += rowValue+":"+hourValue+":"+tempValue+"\n";

                    mqttMessage tempMQTT("tempMQTT", "dbread/out/temp", "10.1.1.11", 1883);
                    tempMQTT.send_message(sOutputStr.c_str());
                    //cout << sOutput.str();
                 }
                 sColName.str(std::string());
                 sColValue.str(std::string());
                 sOutput.str(std::string());
                 sColName.clear();
                 sColValue.clear();
                 sOutput.clear();
             }
             return 0;
         }

         void checkDBErrors()
         {
             if( rc )
             {
                 cout << "DB Error: " << sqlite3_errmsg(db) << endl;
                 closeDB();
             }
         }
	 
         void initDB()
         {
             rc = sqlite3_open("/srv/bmp180/sensordata.db", &db);
             checkDBErrors();
         }

    public:
        DBLite()
        {
            initDB();
	}

        void showTable()
        {
            sql = (char*)"SELECT\
		   row_number() over( order by date_time) rownum,\
		    date_time, temp FROM bmp_data\
                          WHERE date_time BETWEEN datetime('now','-1440 minutes')\
                          AND datetime('now') ;";
            rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
        }

        void closeDB()
        {
            sqlite3_close(db);
        }
};
