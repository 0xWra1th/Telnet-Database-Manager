// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <cstring>
#include <string.h> 
#include <string>
#include <iostream>
#include <regex>
#define PORT 23

using namespace std;

bool notDone = true;
bool localecho = true;
bool serverOn = true;

void insert(){

}

void search(){

} 

void update(){

}

void remove(){

}


void evalCommand(string cmd, int sock){
	char* msg = "";
	if(cmd.substr(0,5).compare("hello") == 0){
    	msg = "\u001b[34mHello, I am a Linux Server talking to you over Telnet!\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    }else if(cmd.substr(0,4).compare("help") == 0){
    	msg = " \u001b[36mManual:\n\thello - Say Hi :)\n\tclear - Clear the terminal\n\tsearch - Search for a record in the database(e.g. search [field] [value])\n\tinsert - Insert a new record into the database(e.g. insert [name] [number])\n\tupdate - Update a record into the database(e.g. update [field] [value] [new_value])\n\tdelete - Delete a record from the database(e.g. delete [field] [value])\n\texit - Exit the program\n\tshutdown - Shutdown the server and exit the program\n\thelp - Display this message\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    }else if(cmd.substr(0,5).compare("clear") == 0){
    	msg = "\033[2J\033[H";
    	send(sock , msg , strlen(msg) , 0 );
    }else if(cmd.substr(0,6).compare("search") == 0){
    	msg = "\u001b[35mResults:\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    }else if(cmd.substr(0,6).compare("insert") == 0){
    	msg = "\u001b[35mNew Record Added!\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    }else if(cmd.substr(0,6).compare("delete") == 0){
    	msg = "\u001b[35mRecord Removed!\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    }else if(cmd.substr(0,6).compare("update") == 0){
    	msg = "\u001b[35mRecord updated!\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    }else if(cmd.substr(0,8).compare("shutdown") == 0){
    	msg = "\u001b[34mServer closing, good bye!\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    	notDone = false;
    	serverOn = false;
    }else if(cmd.substr(0,4).compare("exit") == 0){
    	msg = "\u001b[34mGood Bye!\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    	notDone = false;
    }else{
    	msg = "\u001b[31mInvalid command!\n\u001b[33m";
    	send(sock , msg , strlen(msg) , 0 );
    }
}

int main(int argc, char const *argv[])
{ 
	while(serverOn){
	    //Variable Declarations
	    int server_fd, new_socket, valread; 
	    struct sockaddr_in address; 
	    int opt = 1; 
	    int addrlen = sizeof(address); 
	    char buffer[1024] = {0};
	    char* msg = "\033[2J\033[H\u001b[32m\u001b[1m-------------------\n| Friendly Phones |\n-------------------\u001b[0m\n\u001b[33m";
		
		//Server Setup
		server_fd = socket(AF_INET, SOCK_STREAM, 0);
	    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	    address.sin_family = AF_INET; 
	    address.sin_addr.s_addr = INADDR_ANY; 
	    address.sin_port = htons( PORT ); 
	    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
	    	cerr << "Binding on port " << PORT << " failed!" << endl;
	    	exit(EXIT_FAILURE); 
	    }
		listen(server_fd, 3);
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

	    //Initial Comms
	    valread = read( new_socket , buffer, 1024);
	    send(new_socket , msg , strlen(msg) , 0 );

	    //Main Loop
	    string inputString = "";
	    string word = "";

	    while(notDone){

		    if(localecho){
		    	string output(1024, 0);
			    if (read(new_socket, &output[0], 1023)<0) {
			        cerr << "Failed to read data from socket.\n";
			    }
			    
			    const regex accepted("[^\(\)a-zA-Z0-9_-]");

			    stringstream res;
			    regex_replace(std::ostream_iterator<char>(res), output.begin(), output.end(), accepted, ""); 
			    output = res.str();

			    if(output.length() == 1){
			    	msg = "\u001b[31m\nLocal Echo Off!\n\u001b[33m";
			    	send(new_socket , msg , strlen(msg) , 0 );
			    	localecho = false;
			    	continue;
			    }
			    evalCommand(output, new_socket);
			}else{

				string output(1024, 0);
			    if (read(new_socket, &output[0], 1023)<0) {
			        cerr << "Failed to read data from socket.\n";
			    }
			    
			    const regex accepted("[^\(\)a-zA-Z0-9_ -]");

			    stringstream res;
			    regex_replace(std::ostream_iterator<char>(res), output.begin(), output.end(), accepted, ""); 
			    output = res.str();
			    msg = &output[0];
			    cout << output << ":" << output.length() << endl;
			    if(output.compare("") != 0 && output.compare(" ") != 0){
			    	word = word + output;
				}else if(output.compare("") == 0){
					if(inputString.compare("") != 0){
						inputString = inputString+" "+word;
					}else{
						inputString = word;
					}
					word = "";

					char* inputStrArr = &inputString[0];

					msg = "\n";
					send(new_socket , msg , strlen(msg) , 0 );
					evalCommand(inputString, new_socket);					
			    	inputString = "";
				}else{
					if(inputString.compare(" ") != 0){
						inputString = inputString+" "+word;
					}else{
						inputString = word;
					}
					word = "";
				}


			    if(output.length() > 1){
			    	msg = "\u001b[31m\nLocal Echo On!\n\u001b[33m";
			    	send(new_socket , msg , strlen(msg) , 0 );
			    	localecho = true;
			    	continue;
			    }

			}
	    }
	    shutdown(server_fd, SO_REUSEADDR);
	    close(server_fd);
	}
    
    return 0; 
}