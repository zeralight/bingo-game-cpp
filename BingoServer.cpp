// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string> 
#include <string.h> 
#include <list>
#include <iostream>
#include <pthread.h>
#include <mutex>  
#include "Bingo.h"
using namespace std;

// Function Prototypes
void * handle_client(void * arg);

// Global Variables
int server_fd;
list<Player> players;
list<Game> games;

// Mutexes for functions
pthread_mutex_t playersMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gamesMutex = PTHREAD_MUTEX_INITIALIZER;

/* Registration function for new players
 * Returns true if successful, false otherwise */
bool registerPlayer(string clientMessage)
{
	// Lock players for registration
	pthread_mutex_lock( &playersMutex );
	
	// IMPLEMENT STUB FUNCTION

	// Unlock players after registration complete
	pthread_mutex_unlock( &playersMutex );

	return true;
}

/* Deregistration function for existing players
 * Returns true if successful, false otherwise */
bool deregisterPlayer(string clientMessage)
{
	// Lock players for deregistration
	pthread_mutex_lock( &playersMutex );
	
	// IMPLEMENT STUB FUNCTION

	// 1.) Only allow player deregistration if player specified isn't involved
	//	   in any ongoing bingo games.
	// 2.) Upon deregistration, player will exit the application. 

	// Unlock players after deregistration complete
	pthread_mutex_unlock( &playersMutex );

	return true;
}

string getClientIpAddress(int client_sock)
{
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	int port;

	len = sizeof addr;
	getpeername(client_sock, (struct sockaddr*)&addr, &len);

	// Differentiate between IPv4 and IPv6 addresses
	if (addr.ss_family == AF_INET) {
		struct sockaddr_in *client_sock = (struct sockaddr_in *)&addr;
		port = ntohs(client_sock->sin_port);
		inet_ntop(AF_INET, &client_sock->sin_addr, ipstr, sizeof ipstr);
	} 
	else 
	{
		struct sockaddr_in6 *client_sock = (struct sockaddr_in6 *)&addr;
		port = ntohs(client_sock->sin6_port);
		inet_ntop(AF_INET6, &client_sock->sin6_addr, ipstr, sizeof ipstr);
	}

	return ipstr;
}

Game getGameById(list<Game> gameList, int id)
{
	// Lock games for read
	pthread_mutex_lock( &gamesMutex );
	
	for(list<Game>::const_iterator i = gameList.begin(); i != gameList.end(); i++)
	{			
		Game g = *i;
		if(g.getId() == id) 
		{ 
			// Unlock games after read complete
			pthread_mutex_unlock( &gamesMutex );

			return g;			
		}
	}

	// Unlock games after read complete
	pthread_mutex_unlock( &gamesMutex );

	// Return garbage game
	Game badGame;
	badGame.set_values(-1);
	return badGame;
}

bool removeGameById(list<Game>& gameList, int id)
{
	// Lock games for removal
	pthread_mutex_lock( &gamesMutex );

	for(list<Game>::const_iterator i = gameList.begin(); i != gameList.end(); i++)
	{		
		Game g = *i;	
		if(g.getId() == id) 
		{ 
			gameList.erase(i);

			// Unlock games after removal complete
			pthread_mutex_unlock( &gamesMutex );

			return true;				
		}
	}

	// Unlock games after removal complete
	pthread_mutex_unlock( &gamesMutex );

	return false;
}


int main(int argc, char const *argv[]) 
{ 
	// START TEST DATA

	Player matt;
	matt.set_values("Matt", "127.0.0.1", 8080);
	players.push_back(matt);
	
	Game testGame;
	testGame.set_values(0,"Matt",players);
	games.push_back(testGame);
	
	// END TEST DATA

    // Declare variables
    int new_socket;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[BUFFER_SIZE] = {0}; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Loop forever after creating socket
    do
    {
        // Wait for a client, and then accept
        if((new_socket = accept(server_fd, NULL, NULL)) < 0)
        {
            perror("Failed to accept client");
            close(server_fd);
            exit(1);
        }

        cout << "New client connected\n";

        // Spawn thread to handle the client
        pthread_t threadid;
        pthread_create(&threadid, NULL, handle_client, (void *)&new_socket);
    } while(true);

    return 0; 
} 

/**
 * Handles a single client connection
 */
void * handle_client(void * arg)
{
    int valread; 
    int client_sock = *(int *)arg;
    char buffer[BUFFER_SIZE];
    bool client_connected = true;
    string outputMessage = ""; 
    char temp = '\0';

	// Always handle the client
	while(client_connected)
	{
		// Clear buffer
		memset(buffer, 0, sizeof(buffer));

		// Read socket value
		valread = read( client_sock , buffer, BUFFER_SIZE); 
		printf("Received: %s\n", buffer ); 

		string clientMessage = convertCharArrayToString(buffer);

		// Process command from user
		if(clientMessage.substr(0,2) == REGISTER_NAME)
		{
/*
			// Process registration command
			bool registrationSuccessful = registerPlayer(clientMessage);
			char cstr1[];
   			char delim[]=" ,-;!?";
   			char *token;

	
  			cout << "cstr1 before being tokenized: " << cstr1 << endl << endl;

   			//In the first call to strtok, the first argument is the line to be tokenized
   			token=strtok(cstr1, delim);
   			cout << token << endl;


   			//In subsequent calls to strtok, the first argument is NULL
  			 while((token=strtok(NULL, delim))!=NULL)
   			{
         		cout << token << endl;
   			}


			

			// Send conditional response depending on whether registration was successful
			if(registrationSuccessful)
			{
				outputMessage = string("Registration was successful!");	
			}
			else
			{
				outputMessage = string("Registration was unsuccessful. Please try again.");	
			}

			// Store response 
			const char *outputPointer = outputMessage.c_str();

			// Acknowledge with response
			send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
			printf("Player registered and acknowledgement message sent!\n"); 	*/			
		}
		else if(clientMessage.substr(0,2) == QUERY_PLAYER)
		{
			// Clear output message
			outputMessage = "No players are currently registered.";

			// Convert players list to string and format response
			
			// IMPLEMENT STUB FUNCTION - Implement a function to convert player data to strings
			
			/* for(list<player>::const_iterator i = players.begin(); i != players.end(); i++)
			{			
				outputMessage += i->name.c_str();
				outputMessage += i->address.c_str();
 				outputMessage += to_string(i->port);
				outputMessage += "\n";
			} */

			// printf("%s\n", outputMessage); 	

			// Store response 
			const char *outputPointer = outputMessage.c_str();

			// Acknowledge with response
			send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
			printf("Acknowledgement message sent!\n"); 				
		}
		else if(clientMessage.substr(0,2) == START_GAME)
		{
			// Clear output message
			outputMessage = "";
		
			// IMPLEMENT STUB FUNCTION 	

			// 1.) Search player list for available players and return as string (if <k+1 players on server, return FAILURE)
			// 2.) If >=k+1 players, also append Game ID value to return
			
			// Return failure message until we implement the function
			outputMessage = "FAILURE";

			// Store response 
			const char *outputPointer = outputMessage.c_str();

			// Acknowledge with response
			send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
			printf("Acknowledgement message sent!\n"); 				
		}
		else if(clientMessage.substr(0,2) == QUERY_GAME)
		{
			// Clear output message
			outputMessage = "No games are being played.";

			// Convert games list to string and format response
			
			// IMPLEMENT STUB FUNCTION - Implement a function to convert game data to strings
			
			// 1.) Return code representing the total games in progress.
			// 2.) Return Game ID, Caller Info, and Players Info for each game.

			// Store response 
			const char *outputPointer = outputMessage.c_str();

			// Acknowledge with response
			send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
			printf("Acknowledgement message sent!\n"); 				
		}
		else if(clientMessage.substr(0,2) == END_GAME)
		{
			// Clear output message
			outputMessage = "";

			// Parse command into list
			list<string> clientMessageList = convertStringToList(clientMessage);
			
			// Remove command string from list
			clientMessageList.pop_front();

			// Retrieve game ID from list
			int gameId = stoi(clientMessageList.front());			
			
			// Check if game exists
			Game selectedGame = getGameById(games, gameId);

			// If game exists, verify caller IP address before deletion
			if(selectedGame.getId() != -1)
			{
				string s = getClientIpAddress(client_sock);
				Player caller = selectedGame.getCaller();
				if(caller.getAddress() == s)
				{
					// Client address matches caller address, so remove game from list
					bool removeSuccessful = removeGameById(games, gameId);
					
					if(removeSuccessful) 
					{
						outputMessage = "SUCCESS";
					}
					else
					{				
						outputMessage = "FAILURE";
					}								
				}	
				else
				{
					// Client address doesn't match caller address of game specified
					outputMessage = "FAILURE";
				}
			}
			else
			{
				// Game doesn't exist, so return FAILURE
				outputMessage = "FAILURE";
			}

			// Store response 
			const char *outputPointer = outputMessage.c_str();

			// Acknowledge with response
			send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
			printf("Acknowledgement message sent!\n"); 				
		}
		else if(clientMessage.substr(0,2) == DEREGISTER_NAME)
		{
			// Process deregistration command
			bool deregistrationSuccessful = deregisterPlayer(clientMessage);

			// Send conditional response depending on whether deregistration was successful
			if(deregistrationSuccessful)
			{
				outputMessage = "SUCCESS";	

				// Store response 
				const char *outputPointer = outputMessage.c_str();

				// Acknowledge with response
				send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
				printf("Acknowledgement message sent!\n"); 	

				client_connected = false;
			}
			else
			{
				outputMessage = "FAILURE";	

				// Store response 
				const char *outputPointer = outputMessage.c_str();

				// Acknowledge with response
				send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
				printf("Acknowledgement message sent!\n"); 	
			}			
		}
		else if(clientMessage.substr(0,2) == QUIT)
		{
			client_connected = false;
		}
		else
		{
			// Store response
			outputMessage = string("ACK: ") + buffer;	 
			const char *outputPointer = outputMessage.c_str();

			// Acknowledge with response
			send(client_sock , outputPointer, strlen(outputPointer) , 0 ); 
			printf("Acknowledgement message sent!\n"); 
		} 
	}

    close(client_sock);

    return (void *)0;
}


