#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define CONNECTION_ABORT 10053

#include <cstring> 
#include <string> 
#include <atomic>
#include <cstdlib>
#include <iostream> 
#include <WS2tcpip.h>
#include <winsock2.h>
#include <thread>
#include <mutex>

using namespace std;

atomic_bool running = true;
mutex mtx;

int checkUserInput(int argc, char* argv[]);
void printproperUsage();
int initializeWSA();
SOCKET createSocket();
bool connectToServer(SOCKET clientSocket, char* ipAddr, u_short portNum);
void receiveFromServer(SOCKET clientSocket);