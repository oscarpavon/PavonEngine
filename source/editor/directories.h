#ifndef EDITOR_DIRECTORIES
#define EDITOR_DIRECTORIES

#include <dirent.h>
/* Copy string directory name, 
 * the array need to be allocated previosly
 */
void directory_get_only_directories(const char* path, char * array){

  struct dirent *directory_data; // Pointer for directory entry
  DIR *directory_pointer = opendir(path);

  if (directory_pointer == NULL) {
    LOG("Could not open current directory\n");

    return;
  }
  
	int directory_id = 0;

  while ((directory_data = readdir(directory_pointer)) != NULL) {
		
    if (directory_data->d_name[0] == '.')
      continue;
		if(directory_data->d_type == DT_DIR){
			strcpy(&array[directory_id],directory_data->d_name);
			directory_id++;
		}
	}

  closedir(directory_pointer);

}
#endif
