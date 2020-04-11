#include <stdlib.h>
#include <stdio.h>

#include "../../engine/network/network.h"

int main(int argc, char* argv[]){
	if(argc <= 1)
		return 0;	
	struct NetworkConnecion connection;
	connection.port = 9999;	
	pe_network_connect(&connection);
	pe_net_send_message(&connection,argv[1]);
	return 0;
}
