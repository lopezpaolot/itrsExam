#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define CONNECTION_ABORT 10053
#pragma once

#include <cstring> 
#include <string> 
#include <cstdlib>
#include <iostream> 
#include <WS2tcpip.h>
#include <winsock2.h>

using namespace std;



class chatClient
{
private:
	SOCKET clientSocket;

	int initializeWSA();
	SOCKET createSocket();
	string clientName;

public:
	chatClient();
	void setName(string name);
	string getName();
	
	bool connectToServer(char* ipAddr, u_short portNum);
	string receiveFromServer();
	bool sendToServer(string message);

};

