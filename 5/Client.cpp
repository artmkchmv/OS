#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

int main(int argc, char* argv[]) {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	if (WSAStartup(DLLVersion, &wsaData)) {
		cerr << "Error! WSAStartup load fail!" << '\n';
		return 1;
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(connection, (SOCKADDR*)&addr, sizeofaddr) == SOCKET_ERROR) {
		cerr << "Error! Connection failed!" << '\n';
		closesocket(connection);
		WSACleanup();
		return 1;
	}
	
	int index = 0;
	recv(connection, reinterpret_cast<char*>(&index), sizeof(int), NULL);
	cout << "Student " << index << ".\n";

	HANDLE hMutex = OpenMutexW(SYNCHRONIZE, FALSE, L"mutex");

	if (hMutex == NULL) {
		cerr << "Error! Opening mutex failed! Error code: " << GetLastError() << ".\n";
		closesocket(connection);
		WSACleanup();
		return 1;
	}

	HANDLE pool[2] = {
		OpenEventW(EVENT_ALL_ACCESS, FALSE, L"START"),
		OpenEventW(EVENT_ALL_ACCESS, FALSE, L"END"),
	};

	for (int i = 0; i < 2; ++i) {
		if (!pool[i]) {
			cerr << "Error! Opening event failed! Error code: " << GetLastError() << ".\n";
			closesocket(connection);
			WSACleanup();
			return 1;
		}
	}

	WaitForSingleObject(hMutex, INFINITE);

	SetEvent(pool[0]);

	string msg = "Student " + to_string(index) + " start eating using fork " + to_string((index + 4) % 5) + " and fork " + to_string((index + 1) % 5) + ".\n";
	int msg_sz = msg.size();
	send(connection, (char*)&msg_sz, sizeof(int), NULL);
	send(connection, msg.c_str(), msg_sz, NULL);

	cout << "Eating...\n";
	Sleep(10000);

	SetEvent(pool[1]);

	msg = "Student " + to_string(index) + " stop eating!\n";
	msg_sz = msg.size();
	send(connection, (char*)&msg_sz, sizeof(int), NULL);
	send(connection, msg.c_str(), msg_sz, NULL);

	CloseHandle(hMutex);

	for (int i = 0; i < 2; ++i) {
		CloseHandle(pool[i]);
	}

	if (closesocket(connection) == SOCKET_ERROR) {
		cerr << "Error! Failed to terminate connection. Error code: " << WSAGetLastError() << '\n';
		WSACleanup();
		return 1;
	}

	WSACleanup();

	cout << "Correct exit done!" << '\n';

	return 0;
}
