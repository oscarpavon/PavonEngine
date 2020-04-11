#ifndef EDITOR_NETWORK
#define EDITOR_NETWORK

typedef struct pe_editor_net_command{
	int argument_count;

};

void pe_net_editor_command(struct NetworkConnecion *connection);
#endif
