#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#pragma warning(disable:4996)

using namespace std;

int main(int argc, char* argv[]) {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData)) {
		cout << "Error! WSAStartup load fail!" << '\n';
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);

	for (int i = 1; i <= 1000000; ++i) {
		if (!(connect(Connection, (SOCKADDR*)&addr, sizeof(addr)))) {
			break;
		}
		else {
			cout << i << '\n';
		}
	}

	WSACleanup();
	cout << "Correct exit done!" << '\n';
}
