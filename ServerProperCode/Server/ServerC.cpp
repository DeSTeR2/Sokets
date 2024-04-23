#include "ServerC.h"

ServerC::ServerC(int PORT, int clientNumber) : PORT(PORT), clientNumber(clientNumber) {
	WSADATA ws;
	logg.Init("Log\\Server.txt");
	
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		logg.Log("The wsastartup failed init");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("The wsastartup successfully init");
	}

	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0) {
		logg.Log("The socket didn`t opend");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("The socket opend successfully");
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&srv.sin_zero, 0, 8);

	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)(&nOptVal), nOptLen);
	if (nRet != 0) {
		logg.Log("setsockopt failed");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("setsockopt passed");
	}

	u_long optval = 0; // 0 - if blocking / !0 - non blocking
	nRet = ioctlsocket(nSocket, FIONBIO, &optval);
	if (nRet != 0) {
		logg.Log("ioctlsocket failed");
	}
	else {
		logg.Log("ioctlsocket passed");
	}

	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0) {
		logg.Log("The bind failed");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("The bind done successfully");
	}

	nRet = listen(nSocket, 5); // backlog - how many requests can be pulled at one time, other will wait
	if (nRet < 0) {
		logg.Log("The listen failed");
		exit(EXIT_FAILURE);
	}
	else {
		logg.Log("Started to listen to local port");
	}
}

void ServerC::Start()
{
	nMaxFd = nSocket + 1;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (1) {

		// Clear all fd_set
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		for (int i = 0; i < clientNumber; i++) {
			if (nArrClients[i] != 0) {
				FD_SET(nArrClients[i], &fr);
				FD_SET(nArrClients[i], &fe);
			}
		}


		nRet = select(nMaxFd, &fr, &fw, &fe, &tv);
		if (nRet > 0) {

			//logg.Log("Process data on PORT");
			ProcessNewRequest();


			if (FD_ISSET(nSocket, &fe)) {
				logg.Log("There is an exseption!");
			}

			if (FD_ISSET(nSocket, &fw)) {
				logg.Log("Ready to write data");
			}

			if (FD_ISSET(nSocket, &fr)) {
				logg.Log("Ready to read data");
			}

		}
		else if (nRet == 0) {
			//cout << "Nothing in post: " << PORT << '\n';
		}
		else if (nRet < 0) {
			logg.Log("No connection on select");
		}
	}
}

void ServerC::ProcessNewMessage(int nClientSocket)
{
	//logg.Log("Processing message from client: " + nClientSocket);
	char buff[10001] = { 0 };
	int nRet = recv(nClientSocket, buff, 10001, 0);
	if (nRet <= 0) {
		logg.Log("There is an error! Closing the connection with " + to_string(nClientSocket));
		closesocket(nClientSocket);
		for (int i = 0; i < clientNumber; i++) {
			if (nArrClients[i] == nClientSocket) {
				nArrClients[i] = 0;
				break;
			}
		}
	}
	else {
		string message = buff;

		//if (message.size() > 1) cout << message << endl;
		if (message == "start") {
			cout << "----------------------------------------------------------------------\n";
			logg.Log("Message from client " + to_string(nClientSocket) + ": | " + message + " | ");
			cout << "----------------------------------------------------------------------\n";
			clientsTimes[nClientSocket] = std::chrono::high_resolution_clock::now();
			send(nClientSocket, "R", 1, 0);
			return;
		}
		else
		if (message == "end") {
				//cout << "----------------------------------------------------------------------\n";
				//logg.Log("Message from client: |" + message + "|");
				//cout << "----------------------------------------------------------------------\n";
					
				auto stop = std::chrono::high_resolution_clock::now();
				using namespace std::chrono;
				duration<double> time_span = duration_cast<duration<double>>(stop - clientsTimes[nClientSocket]);
				cout << "Time passed:   " << time_span.count() * 1000 << " miliseconds" << endl;
				int deltaTime = time_span.count() * 1000;
				int messageSize = 13;

				char n_char[20 + sizeof(char)];
				std::sprintf(n_char, "%d", deltaTime);
				send(nClientSocket, n_char, 20 + sizeof(char), 0);

				return;
		}
		else if (message[0] == '\x01') { send(nClientSocket, "L", 1, 0); return; }

		if (commands[message] == "") send(nClientSocket, commands["?"].c_str(), commands["?"].size(), 0);
		else send(nClientSocket, commands[message].c_str(), commands[message].size(), 0);
	}
}

void ServerC::ProcessNewRequest()
{

	// New connection
	if (FD_ISSET(nSocket, &fr)) {
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen);
		if (nClientSocket > 0)
		{
			logg.Log("Accepted request");
			for (int i = 0; i < clientNumber; i++)
			{
				if (nArrClients[i] == 0)
				{
					nArrClients[i] = nClientSocket;
					send(nClientSocket, "Connected!", 10, 0);
					break;
				}
			}
		}
	}
	else {
		for (int i = 0; i < clientNumber; i++) {
			if (FD_ISSET(nArrClients[i], &fr)) {
				ProcessNewMessage(nArrClients[i]);
			}
		}
	}
}

