#include "content_manager.h"
#include "ProjectManager/project_manager.h"
#include "Serialization/json_writer.h"
#include "Textures/texture_factory.h"
#include "Windows/content_browser.h"
#include "editor.h"
#include <string.h>

void content_manager_serialize_static_mesh_values() {
  new_text_token("name", "Content Name");
}

void content_manager_serialize_static_mesh() {
  new_element(content_manager_serialize_static_mesh_values);
}

void content_manager_create_engine_binary(const char *name, ContentType type) {
  LOG("Creating new engine binary\n");
  File brute_file;
  content_GUID_count += 1;

//Export to content directory or project root directory
  char extracted_file_name[50];
  memset(extracted_file_name, 0, sizeof(extracted_file_name));
  path_extract_file_name(name, extracted_file_name);
  LOG("Extracted File name: %s\n", extracted_file_name);
  char new_full_path_with_file_name[500];
  memset(new_full_path_with_file_name, 0, 500);
  strcat(new_full_path_with_file_name, project_manager_current_path);
  
  if (type != CONTENT_TYPE_PROJECT) {
	load_file(name, &brute_file);

    strcat(new_full_path_with_file_name, "/Content/");
    strcat(new_full_path_with_file_name, extracted_file_name);
  } else {
    strcat(new_full_path_with_file_name, "/project    ");//need four space for file extension lines down 
  }
  LOG("New file name in project content folder: \n %s\n",
      new_full_path_with_file_name);

  char new_pavon_binary_path[strlen(name) + 4];
  sprintf(new_pavon_binary_path, "%s", new_full_path_with_file_name);
  sprintf(&new_pavon_binary_path[strlen(new_pavon_binary_path) - 4], "%s", ".pb");

  FILE *engine_binary = fopen(new_pavon_binary_path, "wb");

  // Header
  fprintf(engine_binary, "pvnB");
  u32 version = 1;
  fwrite(&version, sizeof(u32), 1, engine_binary);

  u32 binary_total_size = brute_file.size_in_bytes + 24;
  fwrite(&binary_total_size, sizeof(u32), 1, engine_binary);

  // GUID
  u32 GUID = content_GUID_count;
  fwrite(&GUID, sizeof(u32), 1, engine_binary);

  fwrite(&brute_file.size_in_bytes, sizeof(u32), 1,
         engine_binary); // + engine file JSON

  ContentType new_content_type = type;
  fwrite(&type, sizeof(u32), 1, engine_binary);

  if (type != CONTENT_TYPE_PROJECT) {
		fwrite(brute_file.data, 1, brute_file.size_in_bytes, engine_binary);
  }
  if (type == CONTENT_TYPE_TEXTURE) {
    Image new_image;
    if (image_load_from_memory(&new_image, brute_file.data,
                               brute_file.size_in_bytes) == -1) {

      LOG("ERROR: No image loaded\n");
    } else {
      LOG("Image to engine binary readed\n");
    }
  }

  /*///Thumbnail creation
  content_create_thumbnail(name,CONTENT_TYPE_TEXTURE);
  TextureCreated created_texture = texture_create_to_memory(1,128);
  u32 thumnail_size = (u32)created_texture.size;
  fwrite(&thumnail_size,sizeof(u32),1,engine_binary);
  u32 thumnail_type = 35;
  fwrite(&thumnail_type,sizeof(u32),1,engine_binary);
  fwrite(created_texture.data,created_texture.size,1,engine_binary);
*/
  fclose(engine_binary);
  close_file(&brute_file);
  // remove(name);

  switch (type) {
  case CONTENT_TYPE_TEXTURE: {

    File saved_binary;
    char new_path[strlen(name)];
    sprintf(new_path, "%s", name);
    sprintf(new_path + (strlen(name) - 3), "%s", "pb");
    load_file(new_path, &saved_binary);

    Image new_image;
    if (image_load_from_memory(&new_image, saved_binary.data + 20,
                               saved_binary.size_in_bytes) == -1) {

      LOG("ERROR: Image from binary loaded\n");
    } else {
      LOG("Image readed from binary created\n");
    }
  }
  case CONTENT_TYPE_STATIC_MESH: {
    // FIXME: create function where import
    // and add to viewport
    // content_manager_load_content(glb_path);
    // editor_init_new_added_element();
  }
  }
}

void content_manager_import(const char *path) {
  int name_lenght = strlen(path);
  for (int n = 0; n < name_lenght; n++) {
    if (path[n] == '.') {
      if (strcmp(&path[n + 1], "glb") == 0) {
        content_manager_create_engine_binary(path, CONTENT_TYPE_STATIC_MESH);
        continue;
      }
      if (strcmp(&path[n + 1], "png") == 0) {

        content_manager_create_engine_binary(path, CONTENT_TYPE_TEXTURE);
        continue;
      }
      if (strcmp(&path[n + 1], "pb") == 0) {
        continue;
      }
      LOG("File not support: %s \n", path);
    }
  }
}

void content_manager_init() {

  struct dirent *de; // Pointer for directory entry

  char directory[sizeof(pavon_the_game_project_folder) + 60];
  memset(directory, 0, sizeof(directory));
  sprintf(directory, "%s%s", pavon_the_game_project_folder, "Content/");
  DIR *dr = opendir(directory);

  if (dr == NULL) {
    LOG("Could not open pavon project directory\n");
    return;
  }

  int directory_count = 0;
  while ((de = readdir(dr)) != NULL) {
    directory_count++;
  }

  rewinddir(dr);

  for (int i = 0; i < directory_count; i++) {
    de = readdir(dr);
    sprintf(directory, "%s%s%s", pavon_the_game_project_folder, "Content/",
            de->d_name);

    content_manager_import(directory);
  }

  closedir(dr);
}
