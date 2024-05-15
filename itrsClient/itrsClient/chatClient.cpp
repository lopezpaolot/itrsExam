#include "chatClient.h"


chatClient::chatClient() {
    int wsaErr = initializeWSA();
    if (wsaErr != 0) {
        string errorStr = "Error initializing WSA";
        cout << errorStr << endl;
        throw errorStr;
    }

    clientSocket = createSocket();
    clientName = "";

}

int chatClient::initializeWSA()
{
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);

    return wserr;
}

SOCKET chatClient::createSocket() {
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else {
        std::cout << "Socket is OK!" << std::endl;
    }

    return clientSocket;
}

bool chatClient::connectToServer(char* ipAddr, u_short portNum) {
    // Connect to the server
    sockaddr_in socketInfo;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = inet_addr(ipAddr);  // Replace with the server's IP address
    socketInfo.sin_port = htons(portNum);  // Use the same port as the server

    // Use the connect function
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&socketInfo), sizeof(socketInfo)) == SOCKET_ERROR) {
        std::cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    else {
        std::cout << "Client: Connect() is OK!" << std::endl;
    }

    return true;

}

void chatClient::setName(string name) {
    clientName = name;
}

string chatClient::getName() {
    return clientName;
}

string chatClient::receiveFromServer() {
    char receiveBuffer[200];
    int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
    if (rbyteCount < 0) {
        int wsaError = WSAGetLastError();
        if (wsaError == CONNECTION_ABORT)
            std::cout << "Server Connection Closed";
        else
            std::cout << "Client recv error: " << wsaError << std::endl;
    }
    else {
        string retVal(receiveBuffer);
        return retVal;
    }
    return "";
}

bool chatClient::sendToServer(string message) {

    if (message != "") {
            message = clientName + ": " + message;
        int sbyteCount = send(clientSocket, message.c_str(), 200, 0);
        if (sbyteCount == SOCKET_ERROR) {
            cout << "Client send error: " << WSAGetLastError() << endl;
            return false;
        }
    }
    return true;
}