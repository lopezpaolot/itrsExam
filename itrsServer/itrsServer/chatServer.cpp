#include "chatServer.h"

chatServer::chatServer(string ip_addr, u_short port) {


    server_ip = ip_addr;
    portNum = port;
    clientCounter = 0;
    initializeWSA();
    serverSocket = createSocket();
    bindSocket();
    listenSocket();
    
}

int chatServer::initializeWSA() {
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    return wsaerr;
}

SOCKET chatServer::createSocket() {
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

void chatServer::bindSocket() {
    sockaddr_in socketInfo;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = inet_addr(server_ip.c_str());
    socketInfo.sin_port = htons(portNum);

    // Use the bind function
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&socketInfo), sizeof(socketInfo)) == SOCKET_ERROR) {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
    }
    else {
        std::cout << "bind() is OK!" << std::endl;
    }
}

void chatServer::listenSocket() {
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "listen() is OK! I'm waiting for new connections..." << std::endl;
    }
}

SOCKET chatServer::acceptClientSocket() {
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, nullptr, nullptr);

    if (acceptSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    else {
        std::cout << "accept() is OK!" << std::endl;
        clientSocketVector.push_back(acceptSocket);
        std::cout << "Accept client number " << clientCounter << std::endl;
        clientCounter++;
    }

    return clientSocketVector.back();
}

void chatServer::echoToClients(char* receiveBuffer) {
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

void chatServer::socketCleanup(SOCKET clientSocket) {

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
