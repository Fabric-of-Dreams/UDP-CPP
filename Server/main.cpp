#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
    // Startup Winsock
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        cout << "Can't start Winsock! " << wsOk << endl;
        return;
    }

    // Bind socket to ip address and port
    SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverHint;
    // We use any network card on the machine to get out to the network
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);

    if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
    {
        cout << "Can't bind socket! " << WSAGetLastError() << endl;
    }

    // Information about sender will be here
    sockaddr_in client;
    int clientLength = sizeof(client);
    ZeroMemory(&client, clientLength);

    const int bufsize = 1024;
    char buf[bufsize];

    while (true)
    {
        ZeroMemory(buf, bufsize);

        // Wait for message
        int bytesIn = recvfrom(in, buf, bufsize, 0, (sockaddr*)&client, &clientLength);
        if (bytesIn == SOCKET_ERROR)
        {
            cout << "Error receiving from client " << WSAGetLastError() << endl;
            continue;
        }

        // Display message and client info
        char clientIp[256];
        ZeroMemory(clientIp, 256);

        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        cout << "Message received from " << clientIp << " : " << buf << endl;
    }

    // Close socket
    closesocket(in);

    // Shutdown winsock
    WSACleanup();

    cout << "Hello World!\n";
}