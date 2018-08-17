// (C) Copyright 2018 Dougie Lawson. All rights reserved.
#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <csignal>
#include <cmath>
#include "mqttMessage.hpp"

#define PRESSURE "/sys/bus/iio/devices/iio:device0/in_pressure_input"
#define TEMPERATURE "/sys/bus/iio/devices/iio:device0/in_temp_input"

struct SensorData
{
	float temp;
	float pressure;
};

void sig_handler(int signum)
{
   std::cout << "Received signal " << signum << std::endl;
   std::exit(signum);
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

int main()
{
   std::signal(SIGTERM, sig_handler);

   int rc;

   SensorData values = readBMP180();

   std::string tempStr, pressStr;
   std::stringstream tempSS, pressSS, timeSS;
   std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

   std::time_t tt;
   tt = std::chrono::system_clock::to_time_t(now);

   tempSS << values.temp;
   pressSS << values.pressure;
   timeSS << std::put_time(std::localtime(&tt), "[%a, %d/%m/%Y %T %z] ");

   tempStr = timeSS.str();
   tempStr += tempSS.str();
   tempStr += " °C";

   pressStr = timeSS.str();
   pressStr += pressSS.str();
   pressStr += " hPa";

   mqttMessage tempMQTT("tempMQTT", "test/out/temp", "10.1.1.11", 1883);
   tempMQTT.send_message(tempStr.c_str());
   mqttMessage pressMQTT("pressMQTT", "test/out/pressure", "10.1.1.11", 1883);
   pressMQTT.send_message(pressStr.c_str());

   timeSS.str(std::string());
   tempSS.str(std::string());
   pressSS.str(std::string());
   
   return 0;
}
