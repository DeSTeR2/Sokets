#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <map>
#include <stdio.h>
#include <conio.h>
#include <ratio>

using namespace std;


class ServerC
{
private:

	int PORT;
	int nSocket;
	int clientNumber;
	int nArrClients[10000] = { 0 };
	std::chrono::steady_clock::time_point clientsTimes[10000];
	int nMaxFd = 0;
	int nRet;
	struct sockaddr_in srv;
	fd_set fr, fw, fe;
	struct timeval tv;

	map<string, string> commands = { {"help", "Possible commands: \n start - starts sending messages \n who - tells who am i and variant"}, {"start", "Start sending requests!"}, {"who","Vlasenko Zahar.\n Variant: 22.\nDescription: Message size/speed test."}, {{"?"}, {"Wrong command.\nType - help, to see possible command "}} };

	void ProcessNewMessage(int nClientSocket);
	void ProcessNewRequest();
	void ConnectClient();
	void DoCommand(string command);

public:

	ServerC(int PORT, int clientNumber);
	void Start();
};

