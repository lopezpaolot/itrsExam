
#include "itrsClient.h"

#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char* argv[]) {
    // Initialize WSA variables


    if (checkUserInput(argc, argv) == -1)
        return -1;

    int wsaErr = initializeWSA();
    if (wsaErr != 0) {
        cout << "Error initializing WSA" << endl;
        return -1;
    }

    SOCKET clientSocket = createSocket();
    u_short port = strtoul(argv[2], nullptr, 10);
    if (connectToServer(clientSocket, argv[1], port)) {

        string clientName;
        cout << "Enter your name: ";
        getline(cin, clientName);
        cout << "=================YOU CAN NOW START SENDING MESSAGES=================" << endl;
        thread th1(receiveFromServer, clientSocket);
        //receiveFromServer(clientSocket, &running);


        // Sending data to the server
        
        while (running) {
            string message;
            getline(cin, message);
            
            if (message != "") {
                if (message == "exit") {
                    message = clientName + " logged off";
                    running = false;
                }
                else
                    message = clientName + ": " + message;
                int sbyteCount = send(clientSocket, message.c_str(), 200, 0);
                if (sbyteCount == SOCKET_ERROR) {
                    cout << "Client send error: " << WSAGetLastError() << endl;
                    return -1;
                }
            }
        }

        //closesocket(clientSocket); //let server close socket
        th1.join();

    }



    return 0;
}

int checkUserInput(int argc, char* argv[]) {
    int retVal = 0;

    if (argc != 3) {
        std::cout << "Invalid number of arguments" << std::endl;
        retVal = -1;
    }

    if (inet_addr(argv[1]) == -1) {
        std::cout << "Invalid Server IP" << std::endl;
        retVal = -1;
    }

    for (int i = 0; i < strlen(argv[2]); i++) {
        if (!isdigit(argv[2][i])) {
            std::cout << "Invalid Port Number" << std::endl;
            retVal = -1;
            break;
        }
    }



    if (retVal == -1)
        printproperUsage();


    return retVal;
}

void printproperUsage() {
    std::cout << "Usage: itrsClient.exe <SERVER_IPv4_IP> <PORT_NUM>" << std::endl;
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

bool connectToServer(SOCKET clientSocket, char* ipAddr, u_short portNum) {
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

void receiveFromServer(SOCKET clientSocket) {
    while (running) {
        char receiveBuffer[200];
        int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
        lock_guard<std::mutex> lock(mtx);
        if (rbyteCount < 0) {
            int wsaError = WSAGetLastError();
            if (wsaError == CONNECTION_ABORT)
                std::cout << "Server Connection Closed";
            else
                std::cout << "Client recv error: " << wsaError << std::endl;
        }
        else {
            //std::cout << "Client: Received data: " << receiveBuffer << std::endl;
            std::cout << receiveBuffer << std::endl;
        }
    }
}