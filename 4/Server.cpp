#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <WinSock2.h>

using namespace std;

int main(int argc, char* argv[]) {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData)) {
		cerr << "Error! WSAStartup load fail!" << '\n';
		return 1;
	}

	SOCKADDR_IN addr;
	int sizeOfAddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);

	if (bind(sListen, (SOCKADDR*)&addr, sizeOfAddr) == SOCKET_ERROR) {
		cout << "Error! Bind failed!" << '\n';
		closesocket(sListen);
		WSACleanup();
		return 1;
	}

	if (listen(sListen, 5) == SOCKET_ERROR) {
		cout << "Error! Listen failed!" << '\n';
		closesocket(sListen);
		WSACleanup();
		return 1;
	}

	SOCKET newConnection;
	do {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeOfAddr);
		if (newConnection) {
			closesocket(newConnection);
		}
	} while (newConnection);

	closesocket(sListen);
	WSACleanup();

	return 0;
}
