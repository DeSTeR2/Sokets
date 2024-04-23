#pragma once
#include <iostream>
#include <winsock.h>
#include <string>
#include "Logger.h"

using namespace std;
class ClientC
{
	int nClientSocket;
	struct sockaddr_in srv;
	Logger::Logger logg;
	int _size = 0;
	int messageNumber = 0;

	int sizes[5] = { 10,50,100,1000,10000 };
	std::string messages[4];
	void ProccesMessage(string message);
	void SendRequests(int j);
public: 
	ClientC(int PORT);
	void Start();
};

