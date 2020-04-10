#ifndef LINUX_NETWORK
#define LINUX_NETWORK

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>


typedef struct NetworkConnecion;

void network_print_recieve_data(struct NetworkConnecion* connection);
void network_create_server(struct NetworkConnecion* connection);

#endif
