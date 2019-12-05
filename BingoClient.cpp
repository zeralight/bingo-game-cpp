// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string> 
#include <string.h> 
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include "Bingo.h"
using namespace std;
   
string getUserCommand()
{
	bool commandReceived = false;
	string messageString = "", output = "";
	list<string> stringList;

	while(!commandReceived)
	{
		// Get command input from user
		cout << "Enter a command to send the server or type \"help\" for a list of commands. \n";
		getline(cin, messageString);
		stringList = convertStringToList(messageString);

		// Parse through commands
		if(messageString == "help") 
		{
			printf("1.) register -name -ipAddress -port\n");
			printf("2.) query players\n");
			printf("3.) start game -k\n");
			printf("4.) query games\n");
			printf("5.) end -game-identifier\n");
			printf("6.) deregister -name\n");
			printf("7.) quit\n");
			printf("----------------------------------------\n\n");
		}
		else if(stringList.front() == "register")
		{
			stringList.pop_front();
			
			// Validate number of input arguments
			if(stringList.size() == 3)
			{
				output = REGISTER_NAME + " " + convertListToString(stringList);

				//Declare command received
				commandReceived = true;
			}
		}
		else if(stringList.front() == "query")
		{
			stringList.pop_front();

			if(stringList.front() == "players")
			{
				output = QUERY_PLAYER;

				//Declare command received
				commandReceived = true;
			}
			else if(stringList.front() == "games")
			{
				output = QUERY_GAME;

				//Declare command received
				commandReceived = true;
			}

		}
		else if(stringList.front() == "start")
		{
			stringList.pop_front();

			if(stringList.front() == "game")
			{
				stringList.pop_front();

				// Validate number of input arguments
				if(stringList.size() == 1)
				{
					output = START_GAME + " " + convertListToString(stringList);

					//Declare command received
					commandReceived = true;	
				}			
			}
		}
		else if(stringList.front() == "end")
		{
			stringList.pop_front();

			// Validate number of input arguments
			if(stringList.size() == 1)
			{
				output = END_GAME + " " + convertListToString(stringList);

				//Declare command received
				commandReceived = true;
			}	
		}
		else if(stringList.front() == "deregister")
		{
			stringList.pop_front();

			// Validate number of input arguments
			if(stringList.size() == 1)
			{
				output = DEREGISTER_NAME + " " + convertListToString(stringList);

				//Declare command received
				commandReceived = true;
			}	
		}
		else if(stringList.front() == "quit")
		{
			output = QUIT;

			//Declare command received
			commandReceived = true;
		}
	}

	return output;
}

int main(int argc, char const *argv[]) 
{ 
	bool playing = true;
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[BUFFER_SIZE] = {0}; 
    const char *outputMessage;
    string responseMessage = ""; 
   
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr));    
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }    

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    // Loop forever after creating socket
    do 
    {
		// Clear buffer
		memset(buffer, 0, sizeof(buffer));

		// Get output command to send server
		outputMessage = getUserCommand().c_str();
		string* outputCommand = convertStringToArray(outputMessage);

		// Process command from user
		if(outputCommand[0] == REGISTER_NAME)
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("Registration command sent!\n"); 

			//Process response
			valread = read(sock, buffer, BUFFER_SIZE); 
			printf("RESPONSE: %s\n", buffer );				
		}
		else if(outputCommand[0] == QUIT)
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("Quit command sent!\n"); 				
			close(sock);
			playing = false;
		}
		else if(outputCommand[0] == QUERY_PLAYER)
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("Query command sent!\n"); 

			//Process response
			valread = read(sock, buffer, BUFFER_SIZE); 
			printf("RESPONSE: %s\n", buffer );					
		}
		else if(outputCommand[0] == START_GAME)
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("Start game command sent!\n"); 

			// IMPLEMENT STUB FUNCTION 	

			// 1.) Need to utilize mutex and pthreads to implement
			//     caller/player functionality here.

			//Process response
			valread = read(sock, buffer, BUFFER_SIZE); 
			printf("RESPONSE: %s\n", buffer );					
		}
		else if(outputCommand[0] == QUERY_GAME)
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("Query command sent!\n"); 

			//Process response
			valread = read(sock, buffer, BUFFER_SIZE); 
			printf("RESPONSE: %s\n", buffer );					
		}
		else if(outputCommand[0] == END_GAME)
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("End game command sent!\n"); 

			//Process response
			valread = read(sock, buffer, BUFFER_SIZE); 
			printf("RESPONSE: %s\n", buffer );					
		}
		// Process command from user
		else if(outputCommand[0] == DEREGISTER_NAME)
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("Deregistration command sent!\n"); 

			//Process response
			valread = read(sock, buffer, BUFFER_SIZE); 
			responseMessage = convertCharArrayToString(buffer);

			if(responseMessage == "SUCCESS")
			{
				close(sock);
				playing = false;
			}

			printf("RESPONSE: %s\n", buffer );				
		}
		else
		{
			// Sent output message
			send(sock, outputMessage, strlen(outputMessage), 0 ); 
			printf("Output message sent!\n"); 	

			//Process response
			valread = read(sock, buffer, BUFFER_SIZE); 
			printf("RESPONSE: %s\n", buffer );
		}   
    } 
    while(playing);

    return 0; 
} 

