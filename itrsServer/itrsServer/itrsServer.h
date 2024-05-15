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
#include "chatServer.h"

using namespace std;


vector<thread> clientThreads;
mutex mtx;

void receivefromClient(SOCKET clientSocket);