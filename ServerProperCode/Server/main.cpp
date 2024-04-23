// Server code

#include <iostream>
using namespace std;
#include <winsock.h>
#include "ServerC.h"
#define PORT 9909



int main() {
	ServerC server(PORT, 10);
	server.Start();
	return 0;
}