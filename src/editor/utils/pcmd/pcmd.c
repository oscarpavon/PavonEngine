#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine/network/network.h"

int main(int argc, char* argv[]){
	if(argc <= 1)
		return 0;

	
	char command_buffer[1000];
	memset(command_buffer,0,sizeof(command_buffer));
	for(int i = 1; i < argc; i++){
		strcat(command_buffer,argv[i]);
		if((i+1) == argc)	
			break;

		strcat(command_buffer," ");	
	}
	
	struct NetworkConnecion connection;
	connection.port = 9999;	
	pe_network_connect(&connection);
	pe_net_send_message(&connection,command_buffer);
	return 0;
}
