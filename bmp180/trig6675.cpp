// (C) Copyright 2018 Dougie Lawson. All rights reserved.
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <fstream>
#include <csignal>
#include <cmath>
#include "mqttMessage.hpp"

#define BROKER "10.1.1.11"
#define PORT 1883
#define BOTH_TOPIC "arduino/in/ReadBoth"
#define READC_TOPIC "arduino/in/ReadC"
#define READF_TOPIC "arduino/in/ReadF"

void sig_handler(int signum)
{
   std::cout << "Received signal " << signum << std::endl;
   std::exit(signum);
}

void waitAmin() 
{
	using std::chrono::system_clock;
	std::time_t tt = system_clock::to_time_t (system_clock::now());
	struct std::tm *ptm = std::localtime(&tt);
	ptm->tm_min+=1; ptm->tm_sec=0;
	std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
}

void wait3sec() 
{
	using std::chrono::system_clock;
	std::time_t tt = system_clock::to_time_t (system_clock::now());
	struct std::tm *ptm = std::localtime(&tt);
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

int main()
{
   std::signal(SIGTERM, sig_handler);

   std::string mqttStr;
   mqttStr = "GO";

   waitAmin();

   while (true) 
   {
   	mqttMessage Both6675("mqtt6675", BOTH_TOPIC, BROKER, PORT);
   	Both6675.send_message(mqttStr.c_str());
   	wait3sec();
   	mqttMessage ReadC6675("mqtt6675", READC_TOPIC, BROKER, PORT);
   	ReadC6675.send_message(mqttStr.c_str());
   	wait3sec();
   	mqttMessage ReadF6675("mqtt6675", READF_TOPIC, BROKER, PORT);
   	ReadF6675.send_message(mqttStr.c_str());
   	wait3sec();

   }
   
   return 0;
}
