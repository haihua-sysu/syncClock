//
//  server_2.cpp
//  clockSync
//
//  Created by haihua on 1/24/19.
//  Copyright © 2019 haihua. All rights reserved.
//

#include "lib/time.h"
#include "lib/socket.h"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <signal.h>

using namespace std;

void processFun(ServerSocket server, Socket* client) {
	char buffer[1024] = {0};
	// TODO: need a disconnect signal from client to quit the process
	while (true) {
		read( client->socketfd , buffer, 1024);
		printf("Server: receive msg from processor: %s\n",buffer );

		timeval tv;
		gettimeofday(&tv,NULL);
		string timeMsg = convertTimeToString(tv, 0);
		send(client->socketfd, timeMsg.c_str(), timeMsg.length(), 0);
	}
}

int main(int argc, const char * argv[]) {
    string ip = "127.0.0.1";
    if (argc != 2) {
        printf("Usage: <port>\n");
        _exit(1);
    }
    
    unsigned int port;
    sscanf(argv[1], "%d", &port);
    
    ServerSocket server;
    if (server.Listen(ip.c_str(), port) < 0) {
        puts("sv2 server start error");
        return 0;
    }
    puts("sv2 Server Start\n");
    
    while (true) {
        Socket* client = server.Accept();
        if (client == nullptr) {
			sleep(1);
			continue;
		}
        else {
            thread process(processFun, server, client);
            process.join();
        }
    }
    
    return 0;
}
