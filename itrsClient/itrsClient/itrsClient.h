#include <string> 
#include <atomic>
#include <cstdlib>
#include <iostream> 
#include <thread>
#include <mutex>
#include "chatClient.h"

using namespace std;

atomic_bool running = true;
mutex mtx;

int checkUserInput(int argc, char* argv[]);
void printproperUsage();
void receiveFromServer();