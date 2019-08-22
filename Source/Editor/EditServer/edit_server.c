#include "edit_server.h"
#include "../editor.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

int edit_server_socket;
int edit_server_accept_socket;

void edit_server_proccess_data(){
    
}

void edit_server_create_socket(){
    edit_server_socket = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(7654);

	bind(edit_server_socket,(struct sockaddr*)&address,sizeof(address));

	listen(edit_server_socket,3);

    edit_server_accept_socket = accept(edit_server_socket,(struct sockaddr *)&address,(socklen_t*)&addrlen);
    
    LOG("Edit Server recieve data \n");
    edit_server_proccess_data();
}

void edit_server_init(){
    LOG("Edit Server start\n");
    thread_new_detached(edit_server_create_socket,NULL);
}

void edit_server_finish(){
    close(edit_server_socket);
}
