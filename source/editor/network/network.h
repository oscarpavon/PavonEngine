#ifndef EDITOR_NETWORK
#define EDITOR_NETWORK

typedef struct NetworkConnecion NetworkConnecion;

typedef struct pe_editor_net_command{
	int argument_count;

}pe_editor_net_commnad;

void pe_net_editor_command(NetworkConnecion *connection);
#endif
