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
#include <list>
using namespace std;

// Global definitions
#define PORT 8080 
#define BUFFER_SIZE 1024

// Global operations
#define concat(a, b) a ## b

// Command definitions
const string REGISTER_NAME = "RN";
const string QUERY_PLAYER = "QP";
const string START_GAME = "SG";
const string QUERY_GAME = "QG";
const string END_GAME = "EG";
const string DEREGISTER_NAME = "DN";
const string QUIT = "QT";

// Misc definitions
const string INVALID_PLAYER = "INVALID";

class Player {
    int port;
	string name, address;
  public:
    void set_values (string, string, int);
    void set_values (string);
    string getPlayerString() 
	{
		string result = name + " " + 
						address + " " + 
						to_string(port);
		return result;
	}

	//getters for the name, addres, id, and port for the players
	string getName() const
	{
		return name; 
	}
	string getAddress() const
	{
		return address;
	}
	int getPort() const
	{
		return port;
	}
};

void Player::set_values (string inputName, string inputAddress, int inputPort) {
	name = inputName;
	address = inputAddress;
	port = inputPort;
}

void Player::set_values (string inputId) {
	name = INVALID_PLAYER;
}

class Game {
    int id;
	string callerName;
	list<Player> players;
  public:
    void set_values (int, string, list<Player>);
	void set_values (int);
    string getGameString() 
	{
		// IMPLEMENT STUB FUNCTION TO RETURN GAME TOSTRING()
		string result = "STUB";
		return result;
	}
	Player getCaller()
	{
		for(list<Player>::const_iterator i = players.begin(); i != players.end(); i++)
		{		
			Player p = *i;
			if(p.getName() == callerName) 
			{ 
				return p;				
			}
		}	

		// Return garbage player
		Player badPlayer;
		badPlayer.set_values(INVALID_PLAYER);
		return badPlayer;
	}
	int getId() const
	{
		return id;
	}
	string getCallerName() const
	{
		return callerName;
	}
	list<Player> getPlayers() const
	{
		return players;
	}
};

void Game::set_values (int inputId, string inputCallerName, list<Player> inputPlayerList) {
	id = inputId;
	callerName = inputCallerName;
	players = inputPlayerList;
}

void Game::set_values (int inputId) {
	id = inputId;
}

list<string> convertStringToList(string inputText)
{
    string s = inputText;
	list<string> stringList;
    vector<string> result;
    istringstream iss(s);

    for(string s; iss >> s; )
	{
        stringList.push_back(s);
	}

	return stringList;
}

string convertListToString(list<string> inputList)
{
	ostringstream stream;
	copy(inputList.begin(), inputList.end(), ostream_iterator<string>(stream, " "));
	string result = stream.str();

	// Remove space at the end	
	result = result.substr(0, result.size()-1);

	// Return processed string
	return result;
}

string* convertListToArray(list<string> inputList) {
	string* result = new string[inputList.size()];
	int index = 0;

	// Iterates through the list of strings
	for(list<string>::const_iterator i = inputList.begin(); i != inputList.end(); i++)
	{
		result[index] = i->c_str();
		index++;
	}

 	return result;
}

list<string> convertArrayToList(string* input)
{
	list<string> result;
	size_t i = 0;
    while (!input[i].empty())
	{
		result.push_back(input[i]);
        i++;		
	}
	
	return result;
}

// THIS FUNCTION IS RIDICULOUS... DEAL WITH IT LATER... REALLY...
string* convertStringToArray(string input)
{
    string s = input;
	int size = convertStringToList(input).size(), index = 0;
	string* stringArray = new string[size];
    vector<string> result;
    istringstream iss(s);

    for(string s; iss >> s; )
	{
        stringArray[index] = s;
		index++;
	}

	return stringArray;
}

string convertCharArrayToString(char* input)
{
	string result = "";
	int i = 0;

	while (!input[i] == '\0')
	{
		result += input[i];
		i++;
	}

	return result;
}

int getSizeOfArray(string* input)
{
	int i = 0;
    while (!input[i].empty())
	{
        i++;		
	}
	
	return i;
}


