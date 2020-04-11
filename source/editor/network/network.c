#include "../../engine/network/network.h"
#include "../editor.h"
#include "network.h"

void pe_net_editor_command(struct NetworkConnecion *connection) {
  char buffer[1024] = {0};
  int readed = read(connection->server_in_socket, buffer, 1024);
  if (readed < 0) {
    shutdown(connection->server_in_socket, SHUT_RDWR);
    close(connection->server_in_socket);
    return;
  }
  printf("Net command: %s\n", buffer);
	

}
