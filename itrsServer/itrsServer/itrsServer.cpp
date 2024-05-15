#include "itrsServer.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

chatServer server("127.0.0.1", 55555); // created as global since I cannot pass to thread

int main()
{
    
    // Accept incoming connections
    while (1) {
        SOCKET acceptSocket = server.acceptClientSocket();

        if (acceptSocket != INVALID_SOCKET) {
            thread clienThread(receivefromClient, acceptSocket);
            clientThreads.push_back(move(clienThread));
        }
    }
    return 0;
}



void receivefromClient(SOCKET clientSocket) {

    while (1) {
        char receiveBuffer[200];
        int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
        lock_guard<std::mutex> lock(mtx);
        if (rbyteCount < 0) {
            int errorCode = WSAGetLastError();
            if (errorCode == CONNECTION_CLOSED) {
                server.socketCleanup(clientSocket);
                std::cout << "Client Disconnected" << std::endl;
            }
            else
                std::cout << "Server recv error: " << errorCode << std::endl;
            
            return;
        }

        cout << receiveBuffer << endl;
        server.echoToClients(receiveBuffer);
    }

}
