// Client code

#include <iostream>
#include <winsock.h>
using namespace std;

#define PORT 9909

int nClientSocket;
struct sockaddr_in srv;

int main() {
	WSADATA ws;
	int nRet = 0;
	int nMaxFd;


	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		cout << "The wsastartup failed init\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "The wsastartup successfully init\n";
	}

	nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nClientSocket < 0) {
		cout << "The socket didn`t opend\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "The socket opend successfully\n";
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&srv.sin_zero, 0, 8);


	nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
	if (nRet < 0) {
		cout << "Connection failed\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		cout << "Connected!\n";
	}

	char buff[255] = { 0 };
	recv(nClientSocket, buff, 255, 0);

	cout << "Message: " << buff;
	cout << '\n';
	while (1) {
	}

	return 0;
}