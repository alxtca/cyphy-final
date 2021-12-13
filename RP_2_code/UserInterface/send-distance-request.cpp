//see example below main()
// For local test add /* before this comment and see bottom of code.

#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <iostream>

int sendRequest(char * msg){
	int socket_desc;
	struct sockaddr_in server;
    char message_received[80];
    //to determine sizeof message in send()
	char message_to_send[12];
    strcpy(message_to_send, msg);
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) printf("Could not create socket");
		
	server.sin_addr.s_addr = inet_addr("192.168.0.12"); //replace with server IP. $hostname -I
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		puts("connect error");
		return 1;
	}
	
	puts("Connected");

    //client sends a message
    send(socket_desc, (char*)&message_to_send, sizeof(message_to_send), 0);

    //client receives a message
    puts("awaiting response");
    recv(socket_desc, (char*)&message_received, sizeof(message_received), 0);

    close(socket_desc); //#include unistd.h

    std::string s(message_received);
    int height = std::stoi(s);

    //error codes:
    //10000 - bad request

	return height;
}
// For local test add */ //before this comment


/*
//purpose is
// to demonstrate how to include sendRequest() into main program
#include "send-height-request.h"
#include <iostream>
int main(){
    char request_message[] = "height";
    int height = sendRequest(request_message);
    if (height == 10000){
        std::cout << "bad request" << std::endl;
    } else {
        std::cout << "height is: " << height << std::endl;
    }
    return 0;
}
*/
