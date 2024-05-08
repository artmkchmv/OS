#include <WinSock2.h>
#include <iostream>
#include <vector>

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
    int sizeOfAddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);

    if (bind(sListen, (SOCKADDR*)&addr, sizeOfAddr) == SOCKET_ERROR) {
        cerr << "Error! Bind failed!" << '\n';
        closesocket(sListen);
        WSACleanup();
        return 1;
    }

    if (listen(sListen, 5) == SOCKET_ERROR) {
        cerr << "Error! Listen failed!" << '\n';
        closesocket(sListen);
        WSACleanup();
        return 1;
    }

    HANDLE hMutex = CreateMutexW(NULL, FALSE, L"mutex");

    if (hMutex == NULL) {
        cerr << "Error! Mutex creation failed! Error code: " << GetLastError() << ".\n";
        closesocket(sListen);
        WSACleanup();
        return 1;
    }
   
    HANDLE pool[2] = {
        CreateEventW(NULL, FALSE, FALSE, L"START"),
        CreateEventW(NULL, FALSE, FALSE, L"END"),
    };

    for (int i = 0; i < 2; ++i)
        if (!pool[i]) {
            cerr << "Error! Event creation failed! Error code: " << GetLastError() << ".\n";
            closesocket(sListen);
            WSACleanup();
            return 1;
        }

    int students = 5;
    vector<SOCKET> connections;
    connections.reserve(students);
    int student_number = 0;
    int s_num = 0;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    for (int i = 0; i < students; ++i) {
        if (!CreateProcess(L"C:\\Users\\artem\\source\\repos\\ClientApplication\\x64\\Debug\\ClientApplication.exe",
            NULL,
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi)) {
            cerr << "Error! Process creation failed! Error code: " << GetLastError() << ".\n";
            closesocket(sListen);
            WSACleanup();
            return 1;
        }
    }

    SOCKET newConnection;

    for (int i = 0; i < students; ++i) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeOfAddr);
        if (newConnection == INVALID_SOCKET) {
            cerr << "Error! Accept connection failed!" << '\n';
            closesocket(sListen);
            WSACleanup();
            return 1;
        }
        else {
            connections.push_back(newConnection);
            cout << "Student " << i << " connected!" << '\n';
            send(newConnection, reinterpret_cast<char*>(&i), sizeof(int), NULL);
        }
    }

    while (students) {

        int numEvent = WaitForMultipleObjects(2, pool, FALSE, INFINITE);

        if (numEvent == 0) {

            s_num = student_number;
            ++student_number;

            int msg_sz;
            recv(connections[s_num], (char*)&msg_sz, sizeof(int), NULL);

            char* msg = new char[msg_sz + 1];
            recv(connections[s_num], msg, msg_sz, NULL);
            msg[msg_sz] = '\0';

            cout << msg;

            delete[] msg;
        }
        else {

            int msg_sz;
            recv(connections[s_num], (char*)&msg_sz, sizeof(int), NULL);

            char* msg = new char[msg_sz + 1];
            recv(connections[s_num], msg, msg_sz, NULL);
            msg[msg_sz] = '\0';

            cout << msg;

            delete[] msg;

            ReleaseMutex(hMutex);
            --students;
        }
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    CloseHandle(hMutex);

    for (int i = 0; i < 2; ++i) {
        CloseHandle(pool[i]);
    }

    for (int i = 0; i <= s_num; ++i) {
        closesocket(connections[s_num]);
    }

    if (closesocket(sListen) == SOCKET_ERROR) {
        cerr << "Error! Failed to terminate connection. Error code: " << WSAGetLastError() << '\n';
        WSACleanup();
        return 1;
    }

    WSACleanup();

    cout << "Correct exit done!" << '\n';

    return 0;
}
