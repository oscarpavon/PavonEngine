#ifndef LINUX_NETWORK
#define LINUX_NETWORK

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>


typedef struct NetworkConnecion;

void network_print_recieve_data(struct NetworkConnecion*);
void network_create_server(struct NetworkConnecion*);

void pe_network_connect(struct NetworkConnecion*);
void pe_net_send_message(struct NetworkConnecion*,const char* message);
#endif
