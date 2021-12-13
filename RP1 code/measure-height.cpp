#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "RS-232/rs232.h"
#include <iostream>
#include <string>
#include <ctime>

/* This function sets up a serial connection with Arduino, sending it a message to take a measurement. 
* It waits for a Arduino to finish, and returns the data received back (from Arduino) to main as an int */

#define BUF_SIZE 128

int measureHeight(){
	
	/* -------Setup-------- */
	int COM_port = {24}; // COM port number for device: /dev/ttyACM0 (Arduino MEGA)
	int baud_rate = {115200}; //chosen baud rate
	char mode[] = {'8', 'N', '1'}; //Mode setup: {8 data bits, no parity, 1 stop bit}
	
	char outgoing[BUF_SIZE]; // Buffer for outgoing message
	unsigned char incoming[BUF_SIZE]; // Buffer for incoming message
  
	/* -------Open Serial COM port------- */
	if(RS232_OpenComport(COM_port, baud_rate, mode, 0)) // 0 = no flow control
	//Parameters: (int comport_number, int baudrate, const char * mode, int flowctrl)
	{
		printf("Can not open comport\n");	//print if no connection
	}
	

	/* -------Send bytes-------- */ 
	usleep(1'000'000);  // wait 1 second for stable condition
  	strcpy(outgoing, "start"); //copy "outgoing" buffer to string
	RS232_cputs(COM_port, outgoing); // sending "outgoing" string to Arduino
	printf("Message sent to Arduino: '%s'\n", outgoing); //Feedback: Showing data sent to Arduino
	
		
	/* -------Wait for reply------- */
	int bytes_received{0};
	time_t current_time; 	//time object
	time_t later_time;		//time object
	time(&current_time); 	//function: add current time to object
	
	//Polling loop
	while(bytes_received == 0){
		time(&later_time);	//new current time
		bytes_received = RS232_PollComport(COM_port, incoming, (int)BUF_SIZE); //int received shows number of bytes received from Arduino
		//Parameters:(int comport_number, unsigned char *buf, int size)
		
		if(bytes_received > 0){ // If 'received' is greater than zero, then there was data from Arduino
			incoming[bytes_received] = 0;   // add zero behind last received byte to signals end of data
			printf("Received %i bytes: '%s'\n", bytes_received, (char *)incoming); //Feedback: Showing number of bytes received
			break;
		}
		/*
		if((long)later_time >= (long)current_time + 15){ // arduino should reply within 10 seconds, if 15 seconds pass, something is wrong
			std::cout << "No reply from arduino" << std::endl;
			return 0;
		}
		*/
	}
	// data reveived
	time(&later_time); //new current time
	std::cout << "Arduino replied after " <<
		((long)later_time - (long)current_time) << " seconds" << std::endl;
		
		
	/* -------Close port------ */
	std::cout << "Closing port" << std::endl;
	RS232_CloseComport(COM_port);
	
	
	/* ------Convert received data to int------ */
	char char_copy[BUF_SIZE];
	// Convert from unsigned to normal char
	for(int i = 0 ; i < BUF_SIZE; i++){
		char_copy[i] = incoming[i];		
		if(incoming[i] == '\0'){
			break;
		}
	}
	
	std::string string_copy{char_copy}; //convert char to string
	int distance{std::stoi(string_copy)}; //convert string to int
	//Test: Feedback to user, is distance correct?
	std::cout << "Distance to object: " << distance << " cm" << std::endl;
  
  return distance;
}

