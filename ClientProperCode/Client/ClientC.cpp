#include "ClientC.h"

void ClientC::ProccesMessage(string message)
{
	if (message == "R") {
		SendRequests(messageNumber);
		messageNumber = (messageNumber + 1) % 4;
		cout << '\n';

	}
}

void ClientC::SendRequests(int j)
{
	logg.Log("Request size: " + to_string(messages[j].size()) + " bits");
	while (1) {

		char buff[10005] = { 0 };
		logg.Log("Sending " + to_string(sizes[_size]) + " requests");

		for (int i = 0; i < sizes[_size]; i++) {
			//for (int j = 0; j < 4; j++) {
			//for (int q = 0; q < messages[j].size(); q++) {
			send(nClientSocket, messages[j].c_str(), messages[j].size(), 0);
			int p = i + 1;
			//cout << 1 << endl;
			recv(nClientSocket, buff, 10005, 0);
			//}
			//	system("stop");
			//}
		}
		memset(buff, 0, 10005);
		send(nClientSocket, "end", 3, 0);
		recv(nClientSocket, buff, 10005, 0);
		string time_ = buff;
		time_ = "Passed " + time_ + " miliseconds";
		logg.Log(time_);
		_size = (_size + 1) % 5;
		if (_size == 0) break;
	}
}

ClientC::ClientC(int PORT)
{
	srand(time(0));
	logg.Init("Log\\Client" + to_string(rand() % 100) + ".txt");

	int size = 10;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < size; j++) messages[i] += '\x01';
		size *= 10;
	}

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
}

void ClientC::Start()
{
	char buff[255] = { 0 };
	recv(nClientSocket, buff, 255, 0);

	std::string str = buff;
	logg.Log("Message: " + str);
	while (1) {
		memset(buff, 0, sizeof buff);
		std::string input;
		logg.Log("Waiting for command: ");
		cin >> input;

		logg.Log("Command: " + input);
		if (input == "stop") break;

		send(nClientSocket, input.c_str(), input.size(), 0);
		recv(nClientSocket, buff, 255, 0);
		logg.Log(buff);
		ProccesMessage(buff);
	}
	logg.~Logger();
}
