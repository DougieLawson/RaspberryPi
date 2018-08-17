#include <mosquittopp.h>
#include <mosquitto.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <cstring>
#include "mqttMessage.hpp"

mqttMessage::mqttMessage(const char * _id,const char * _topic, const char * _host, int _port) : mosquittopp(_id)
{
        mosqpp::lib_init();
        this->keepalive = 60;
        this->id = _id;
        this->port = _port;
        this->host = _host;
        this->topic = _topic;
        connect_async(host, port, keepalive);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
};

mqttMessage::~mqttMessage()
{
        mosqpp::lib_cleanup();
}

bool mqttMessage::send_message(const  char* _message)
{
        int ret = publish(NULL,this->topic,strlen(_message),_message,1,false);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
        disconnect();
        return ( ret == MOSQ_ERR_SUCCESS );
}

void mqttMessage::on_disconnect(int rc)
{
        //std::cout << ">> mqttMessage - disconnection(" << rc << ")" << std::endl;
        loop_stop(); // we're not using loop_start()
}

void mqttMessage::on_connect(int rc)
{
        if ( rc == 0 )
        {
                //std::cout << ">> mqttMessage - connected with server" << std::endl;
        	loop_start();  // this blocks after a publish
        }
        else
        {
                std::cout << ">> mqttMessage - Impossible to connect with server(" << rc << ")" << std::endl;
        }
}

void mqttMessage::on_publish(int mid)
{
        //std::cout << ">> mqttMessage - Message (" << mid << ") succeed to be published " << std::endl;
}
