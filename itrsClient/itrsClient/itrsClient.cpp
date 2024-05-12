
#include "itrsClient.h"

#pragma comment(lib, "Ws2_32.lib")


int main() {
    // Initialize WSA variables
    int wsaErr = initializeWSA();
    if (wsaErr != 0) {
        std::cout << "Error initializing WSA" << std::endl;
        return -1;
    }

    SOCKET clientSocket = createSocket();
    connectToServer(clientSocket, "127.0.0.1", 55555);

    std::string clientName;
    std::cout << "Enter your name: ";
    std::getline(std::cin, clientName);
    std::cout << "=================YOU CAN NOW START SENDING MESSAGES=================" << std::endl;
    
    std::thread th1(receiveFromServer, clientSocket, clientName);


    // Sending data to the server
    while (1) {
        std::string message;
        std::getline(std::cin, message);
        if (message != "") {
            message = clientName + ": " + message;
            int sbyteCount = send(clientSocket, message.c_str(), 200, 0);
            if (sbyteCount == SOCKET_ERROR) {
                std::cout << "Client send error: " << WSAGetLastError() << std::endl;
                return -1;
            }
        }
    }



    return 0;
}

int initializeWSA()
{
    WSADATA wsaData;
    int wserr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wserr = WSAStartup(wVersionRequested, &wsaData);

    return wserr;
}

SOCKET createSocket() {
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

void connectToServer(SOCKET clientSocket, std::string ipAddr, u_short portNum) {
    // Connect to the server
    sockaddr_in socketInfo;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = inet_addr(ipAddr.c_str());  // Replace with the server's IP address
    socketInfo.sin_port = htons(portNum);  // Use the same port as the server

    // Use the connect function
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&socketInfo), sizeof(socketInfo)) == SOCKET_ERROR) {
        std::cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    else {
        std::cout << "Client: Connect() is OK!" << std::endl;
    }

}

void receiveFromServer(SOCKET clientSocket, std::string clientName) {
    while (1) {
        char receiveBuffer[200];
        int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
        if (rbyteCount < 0) {
            std::cout << "Client recv error: " << WSAGetLastError() << std::endl;
        }
        else {
            //std::cout << "Client: Received data: " << receiveBuffer << std::endl;
            std::cout << receiveBuffer << std::endl;
        }
    }
}