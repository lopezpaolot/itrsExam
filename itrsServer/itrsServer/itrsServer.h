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

vector<SOCKET> clientSocketVector;
vector<thread> clientThreads;
int clientCounter = 0;
mutex mtx;

int initializeWSA();
SOCKET createSocket();
void bindSocket(SOCKET socket, string ipAddr, u_short portNum);
void listenSocket(const SOCKET& serverSocket);

SOCKET acceptClientSocket(SOCKET serverSocket);
void echoToClient(char* receiveBuffer);
void receivefromClient(SOCKET clientSocket);
void socketCleanup(SOCKET clientSocket);