//live server idea is from:
//https://github.com/Mad-scientist-Monkey/sockets-ccpp-rpi

#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include <iostream>
#include "measure-height.h"

int main(){
    char message_received[80];
	int socket_desc , new_socket , c;
	struct sockaddr_in server , client;
	//char *message;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) printf("Could not create socket");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
		puts("bind failed");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
		puts("Connection accepted");
		
        //server receives a message
        std::cout << "awaiting clients message" << std::endl;
        memset(&message_received,0,sizeof(message_received));//clear buffer
        recv(new_socket, (char*)&message_received, sizeof(message_received), 0);
        if (!strcmp(message_received, "height")) {
            std::cout << "going to call measure height" << std::endl;
            char const *message_to_send = std::to_string(measureHeight()).c_str();
            send(new_socket,message_to_send, sizeof(message_to_send),0);
            std::cout << "height has been sendt" << std::endl;
        }
        else{
            char reply_msg[] = "10000";
            send(new_socket,reply_msg, sizeof(reply_msg),0);
            std::cout << "unknown req. has been sendt" << std::endl;
        }
	}
	
	if (new_socket<0){
		perror("accept failed");
		return 1;
	}
	
	return 0;
}
// g++ main.cpp measure-height.cpp RS-232/rs232.c -Wall -Wextra -o2 -o rp1main
