#include "edit_server.h"
#include "../editor.h"
#include "../../engine/network/network.h"

int edit_server_socket;
int edit_server_accept_socket;

void edit_server_proccess_data(){
    char buffer[1024] = {0}; 
    int readed = read( edit_server_accept_socket , buffer, 1024);
    if(readed < 0){
        shutdown(edit_server_accept_socket,SHUT_RDWR);
        close(edit_server_accept_socket);
        return;
    } 
    printf("%s\n",buffer ); 
    char path[strlen(pavon_the_game_project_folder) + 30];
    sprintf(path,"%s%s%s%s",pavon_the_game_project_folder,"Content/",buffer,".glb");
    LOG("%s\n",path);

    EditorCommand new_command;
    memset(&new_command, 0, sizeof(EditorCommand));
    new_command.command = content_manager_import;
    strcpy(new_command.command_text, path);
    array_add(&array_editor_command_queue,&new_command);
    
}

void edit_server_create_socket(){
    edit_server_socket = socket(AF_INET,SOCK_STREAM,0);
    if (edit_server_socket < 0){
        shutdown(edit_server_socket,SHUT_RDWR);
        close(edit_server_socket);
        LOG("ERROR opening socket");
        return;
    } 

    if (setsockopt(edit_server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0){
        LOG("setsockopt(SO_REUSEADDR) failed");
        shutdown(edit_server_socket,SHUT_RDWR);
        close(edit_server_socket);
        return;
    }
        
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(7654);

	if ( bind(edit_server_socket,(struct sockaddr*)&address,sizeof(address)) < 0){
        LOG("Fail to bind sockect\n");
        shutdown(edit_server_socket,SHUT_RDWR);
        close(edit_server_socket);
        return;
    } 

    

	listen(edit_server_socket,3);

    while(editor_running){
        edit_server_accept_socket = accept(edit_server_socket,(struct sockaddr *)&address,(socklen_t*)&addrlen);
        if(!editor_running)
            return;    
        LOG("Edit Server recieve data \n");
        edit_server_proccess_data();
    }
    shutdown(edit_server_accept_socket,SHUT_RDWR);
    close(edit_server_accept_socket);    
}

void edit_server_init(){
    LOG("Edit Server start\n");
    thread_new_detached(edit_server_create_socket,NULL,"EditServer");

		struct NetworkConnecion new_connection;
		memset(&new_connection,0,sizeof(struct NetworkConnecion));
		new_connection.port = 9999;
		new_connection.server_running = 1;
		new_connection.data_process = &network_print_recieve_data;
		strcat(new_connection.name,"CMD connection");	
		thread_new_detached(network_create_server,&new_connection,"CMD server");
}

void edit_server_finish(){
    shutdown(edit_server_socket,SHUT_RDWR);
    close(edit_server_socket);

    shutdown(edit_server_accept_socket,SHUT_RDWR);
    close(edit_server_accept_socket); 
}
