#include "path.h"
#include "../engine/types.h"
#include <string.h>
void path_extract_file_name(const char *file_name, char *buf) {
	bool found = false;
  int name_lenght = strlen(file_name);
  for (int n = name_lenght; n > 0; n--) {
    if (file_name[n] == '/') {
      strcpy(buf, &file_name[n + 1]);
      break;
    }
  }
	if(!found)
      strcpy(buf, file_name);
}


