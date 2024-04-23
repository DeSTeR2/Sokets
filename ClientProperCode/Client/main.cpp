// Client code

#include "ClientC.h"
using namespace std;

#define PORT 9909


int main() {
	ClientC client(PORT);
	client.Start();
	return 0;
}