#include "DBLite.hpp"

int main()
{
        DBLite sensor;
        //cout << "{\"bmp_data\": [" << endl; 
        sensor.showTable();
        sensor.closeDB();
        //cout << "]}" << endl;

        return 0;
}
