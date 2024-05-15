#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define CONNECTION_CLOSED 10054

#include <cstring>
#include <string>
#include <vector>
#include <iostream> 
#include <WS2tcpip.h>
#include <winsock2.h>
#include <thread>
#include <mutex>

using namespace std;
class chatServer
{
private:
	SOCKET serverSocket;
	string server_ip;
	u_short portNum;
	vector<SOCKET> clientSocketVector;
	int clientCounter;

	int initializeWSA();
	SOCKET createSocket();
	void bindSocket();
	void listenSocket(); 


public:
	chatServer(string ip_addr, u_short port);
	SOCKET acceptClientSocket();
	void echoToClients(char* receiveBuffer);
	void socketCleanup(SOCKET clientSocket);
	


};

