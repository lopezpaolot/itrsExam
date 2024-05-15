
#include "itrsClient.h"

#pragma comment(lib, "Ws2_32.lib")
chatClient client; // created as global since I cannot pass to thread


int main(int argc, char* argv[]) {

    if (checkUserInput(argc, argv) == -1)
        return -1;

    u_short port = strtoul(argv[2], nullptr, 10);
    chatClient client;
    if (client.connectToServer(argv[1], port)) {
        string clientName;
        cout << "Enter your name: ";
        getline(cin, clientName);

        client.setName(clientName);
        cout << "=================YOU CAN NOW START SENDING MESSAGES=================" << endl;

        thread th1(receiveFromServer);

        // Sending data to the server

        while (running) {
            string message;
            getline(cin, message);

            if (message != "") {
                if (message == "exit") {
                    message = "logged off";
                    running = false;
                }
                
                if (!client.sendToServer(message))
                    cout << "Message failed to send" << endl;
            }
        }

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

void receiveFromServer() {
    while (running) {
        string message = client.receiveFromServer();
        lock_guard<std::mutex> lock(mtx);
        std::cout << message << std::endl;
    }
}