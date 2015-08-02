#pragma once
#ifndef IRCCONNECTION
#define IRCCONNECTION
#define DEFAULT_BUFFER 40960 //buffer overflow exploit????
#include <string>
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <boost\algorithm\string.hpp>
#include <vector>
using namespace std;

class IrcConnection {
private:
	char server_addr[1024];
	int port;
	string channel;
	string nick;
	SOCKET client;
	void(*callback)(string);
	void handleIRCMessage(string message);

public:
	IrcConnection();
	IrcConnection(string _server, int _port, string _channel, void(*_callback)(string));
	IrcConnection(string _server, int _port, string _channel, void(*_callback)(string), string _nick);
	bool connectToServer();
	void read();
	void join(string channel);
	void sendMessage(string _message);
};



#endif