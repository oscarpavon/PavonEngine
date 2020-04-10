#include "../../network/network.h"
#include "../../log.h"
void network_create_server(struct NetworkConnecion* connection){
    connection->server_socket = socket(AF_INET,SOCK_STREAM,0);
    if (connection->server_socket< 0){
        shutdown(connection->server_socket,SHUT_RDWR);
        close(connection->server_socket);
        LOG("ERROR opening socket");
        return;
    } 

    if (setsockopt(connection->server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0){
        LOG("setsockopt(SO_REUSEADDR) failed");
        shutdown(connection->server_socket,SHUT_RDWR);
        close(connection->server_socket);
        return;
    }
        
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(connection->port);

	if ( bind(connection->server_socket,(struct sockaddr*)&address,sizeof(address)) < 0){
        LOG("Fail to bind socket form connection %s \n",connection->name);
        shutdown(connection->server_in_socket,SHUT_RDWR);
        close(connection->server_socket);
        return;
    } 

    

	listen(connection->server_socket,3);

    while(connection->server_running = 1){
        connection->server_in_socket= accept(connection->server_socket,(struct sockaddr *)&address,(socklen_t*)&addrlen);
        if(connection->server_running != 1)
            return;    
				connection->data_process(connection); 
    }
    shutdown(connection->server_socket,SHUT_RDWR);
    close(connection->server_in_socket);    
}

void network_print_recieve_data(struct NetworkConnecion* connection){
    char buffer[1024] = {0}; 
    int readed = read( connection->server_in_socket, buffer, 1024);
    if(readed < 0){
        shutdown(connection->server_in_socket,SHUT_RDWR);
        close(connection->server_in_socket);
        return;
    } 
    printf("%s\n",buffer ); 

}
