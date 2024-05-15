#include "itrsServer.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;



int main()
{
    int MAX_CLIENTS = 10;
    int wsaErr = initializeWSA();
    if (wsaErr != 0) {
        std::cout << "Error initializing WSA" << std::endl;
        return -1;
    }


    SOCKET serverSocket = createSocket();
    bindSocket(serverSocket, "127.0.0.1", 55555);
    listenSocket(serverSocket);


    // Accept incoming connections
    while (1) {
        SOCKET acceptSocket = acceptClientSocket(serverSocket);

        if (acceptSocket != INVALID_SOCKET) {
            clientSocketVector.push_back(acceptSocket);
            thread clienThread(receivefromClient, clientSocketVector[clientCounter]);
            clientThreads.push_back(move(clienThread));
            std::cout << "Accept client numer " << clientCounter << std::endl;
            clientCounter++;
        }
    }
    return 0;
}

int initializeWSA() {
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    return wsaerr;
}

SOCKET createSocket() {
    SOCKET createdSocket;
    createdSocket = INVALID_SOCKET;
    createdSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (createdSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    else {
        std::cout << "Socket is OK!" << std::endl;
    }

    return createdSocket;
}

void bindSocket(SOCKET socket, string ipAddr, u_short portNum) {
    sockaddr_in socketInfo;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    socketInfo.sin_port = htons(portNum);

    // Use the bind function
    if (bind(socket, reinterpret_cast<SOCKADDR*>(&socketInfo), sizeof(socketInfo)) == SOCKET_ERROR) {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(socket);
        WSACleanup();
    }
    else {
        std::cout << "bind() is OK!" << std::endl;
    }
}

void listenSocket(const SOCKET& serverSocket) {
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "listen() is OK! I'm waiting for new connections..." << std::endl;
    }
}

SOCKET acceptClientSocket(SOCKET serverSocket) {
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, nullptr, nullptr);

    if (acceptSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    else {
        std::cout << "accept() is OK!" << std::endl;
    }

    return acceptSocket;
}

void echoToClient(char* receiveBuffer) {
    for (unsigned int i = 0; i < clientSocketVector.size(); i++) {
        SOCKET clientSocket = clientSocketVector[i];
        int sbyteCount = send(clientSocket, receiveBuffer, 200, 0);
        if (sbyteCount == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            if (errorCode == CONNECTION_CLOSED) {
                socketCleanup(clientSocket);
                std::cout << "Client Disconnected" << std::endl;
            }
            else
                std::cout << "Server send error: " << errorCode << std::endl;
            return;
        }
    }
}

void receivefromClient(SOCKET clientSocket) {

    while (1) {
        char receiveBuffer[200];
        int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
        lock_guard<std::mutex> lock(mtx);
        if (rbyteCount < 0) {
            int errorCode = WSAGetLastError();
            if (errorCode == CONNECTION_CLOSED) {
                socketCleanup(clientSocket);
                std::cout << "Client Disconnected" << std::endl;
            }
            else
                std::cout << "Server recv error: " << errorCode << std::endl;
            
            return;
        }

        cout << receiveBuffer << endl;
        echoToClient(receiveBuffer);
    }

}

void socketCleanup(SOCKET clientSocket) {
    
    closesocket(clientSocket);
    std::vector<SOCKET>::iterator it = clientSocketVector.begin();

    for (; it != clientSocketVector.end(); ++it) {
        if (*it == clientSocket) {
            clientSocketVector.erase(it);
            clientCounter--;
            break;
        }
    }

}
