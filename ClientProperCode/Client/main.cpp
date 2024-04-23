// Client code

#include <iostream>
#include <winsock.h>
#include "Logger.h"
using namespace std;

#define PORT 9909

int nClientSocket;
struct sockaddr_in srv;
Logger::Logger logg;


int main() {
	logg.Init("Log\\Client.txt");

	WSADATA ws;
	int nRet = 0;
	int nMaxFd;


	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		logg.Log("The wsastartup failed init");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("The wsastartup successfully init");
	}

	nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nClientSocket < 0) {
		logg.Log("The socket didn`t opend");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("The socket opened successfully");
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&srv.sin_zero, 0, 8);


	nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
	if (nRet < 0) {
		logg.Log("Connection failed");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("Connected!");
	}

	char buff[255] = { 0 };
	recv(nClientSocket, buff, 255, 0);

	std::string str = buff;
	logg.Log("Message: " + str);
	while (1) {
	}

	return 0;
}