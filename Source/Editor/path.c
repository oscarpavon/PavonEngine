#include "path.h"
void path_extract_file_name(const char *file_name, const char *buf) {

  int name_lenght = strlen(file_name);
  for (int n = name_lenght; n > 0; n--) {
    if (file_name[n] == '/') {
      strcpy(buf, &file_name[n + 1]);
      break;
    }
  }
}



