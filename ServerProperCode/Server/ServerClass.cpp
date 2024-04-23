/*#include "ServerClass.h"

ServerClass::ServerClass(int PORT, int clientNumber): PORT(PORT), clientNumber(clientNumber) {
	WSADATA ws;
	

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		cout << "The wsastartup failed init\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "The wsastartup successfully init\n";
	}

	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0) {
		cout << "The socket didn`t opend\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "The socket opend successfully\n";
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&srv.sin_zero, 0, 8);

	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)(&nOptVal), nOptLen);
	if (nRet != 0) {
		cout << "setsockopt failed\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "setsockopt passed\n";
	}

	u_long optval = 0; // 0 - if blocking / !0 - non blocking
	nRet = ioctlsocket(nSocket, FIONBIO, &optval);
	if (nRet != 0) {
		cout << "ioctlsocket failed\n";
	}
	else {
		cout << "ioctlsocket passed\n";
	}

	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0) {
		cout << "The bind failed\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "The bind done successfully\n";
	}

	nRet = listen(nSocket, 5); // backlog - how many requests can be pulled at one time, other will wait
	if (nRet < 0) {
		cout << "The listen failed\n";
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Started to listen to local port\n";
	}
}

void ServerClass::Start()
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

			cout << "Process data on PORT\n";
			ProcessNewRequest();


			if (FD_ISSET(nSocket, &fe)) {
				cout << "There is an exseption!\n";
			}

			if (FD_ISSET(nSocket, &fw)) {
				cout << "Ready to write data\n";
			}

			if (FD_ISSET(nSocket, &fr)) {
				cout << "Ready to read data \n";
			}

		}
		else if (nRet == 0) {
			//cout << "Nothing in post: " << PORT << '\n';
		}
		else if (nRet < 0) {
			cout << "No connection on select\n";
		}
	}
}

void ServerClass::ProcessNewMessage(int nClientSocket)
{
	cout << "Processing message from client: " << nClientSocket << '\n';
	char buff[256 + 1] = { 0 };
	int nRet = recv(nClientSocket, buff, 256, 0);
	if (nRet <= 0) {
		cout << "There is an error! Closing the connection\n";
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
		if (message == "Start1") { 
			cout << "----------------------------------------------------------------------\n";
			cout << "Message from client: |" << message << "|\n";
			cout << "----------------------------------------------------------------------\n";
			clientsTimes[nClientSocket] = std::chrono::high_resolution_clock::now();
			send(nClientSocket, "R", 1, 0);
			return;
		} else 
		if (message == "End") {
			cout << "----------------------------------------------------------------------\n";
			cout << "Message from client: |" << message  << "|\n";
			cout << "----------------------------------------------------------------------\n";
			auto stop = std::chrono::high_resolution_clock::now();
			using namespace std::chrono;
			duration<double> time_span = duration_cast<duration<double>>(stop - clientsTimes[nClientSocket]);
			cout << time_span.count() * 1000 << endl;
			int deltaTime = time_span.count() * 1000;
			int messageSize = 13;

			char n_char[20 + sizeof(char)];
			std::sprintf(n_char, "%d", deltaTime);
			send(nClientSocket, n_char, 20 + sizeof(char), 0);

			return;
		} 

		if (commands[message] == "") send(nClientSocket, commands["?"].c_str(), commands["?"].size(), 0);
		else send(nClientSocket, commands[message].c_str(), commands[message].size(), 0);
	}
}

void ServerClass::ProcessNewRequest()
{

	// New connection
	if (FD_ISSET(nSocket, &fr)) {
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen);
		if (nClientSocket > 0)
		{
			cout << "Accepted request\n";
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

*/