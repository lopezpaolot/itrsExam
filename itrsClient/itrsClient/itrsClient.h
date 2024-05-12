#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstring> 
#include <string> 
#include <cstdlib>
#include <iostream> 
#include <WS2tcpip.h>
#include <winsock2.h>
#include <thread>


int initializeWSA();
SOCKET createSocket();
void connectToServer(SOCKET clientSocket, std::string ipAddr, u_short portNum);
void receiveFromServer(SOCKET clientSocket, std::string clientName);