#include "images.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdParty/stb_image.h"

#include "engine.h"

Texture* texture_current_to_load;
int image_load_from_memory(Image* image,void* data, u32 size){	
    int width, height, comp, req_comp;
    req_comp = 3;

    unsigned char* decoded = stbi_load_from_memory(data, size, &width, &height, &comp, req_comp);
	if(decoded == NULL){
	LOG("[X]=========ERROR: Image not decoded\n");
		return -1;
	}
    image->heigth = (unsigned short)height;
    image->width = (unsigned short)width;
    image->pixels_data = decoded;
    return 0;
}

int image_load(const char* path, Image* image){
  File new_file;
  if (load_file(path, &new_file) == -1) {
    LOG("Image not loaded: %s\n", path);
    return -1;
  }
  image_load_from_memory(image, new_file.data, new_file.size_in_bytes);
  close_file(&new_file);
  return 0;
}

int texture_load_from_memory(Texture* texture,u32 size,void* data){
	memset(texture,0,sizeof(Texture));
	texture_current_to_load = texture;
	if(image_load_from_memory(&texture->image,data,size)== -1){
		return -1;
	}

	load_texture_to_GPU(texture_current_to_load);
		
	return 0;
}

int texture_load(const char *path, Texture *new_texture) {
  texture_current_to_load = new_texture;
  if (image_load(path, &new_texture->image) == -1) {
    new_texture->id = 0;
    return -1;
  }

//  load_texture_to_GPU(texture_current_to_load);
	pe_th_exec_in(pe_th_render_id,&load_texture_to_GPU,texture_current_to_load);
}

int load_image_with_format(const char* path, GLint format, Image* out_image){
    File new_file;

    int result = load_file(path,&new_file);
    if(result == -1)
        return -1;

    Image new_image;

    int width, height, comp, req_comp;
    if(format == GL_RGB)
        req_comp = 3;

    if(format == GL_RGBA)
        req_comp = 4;

    unsigned char* decoded = stbi_load_from_memory(new_file.data, (int)new_file.size_in_bytes, &width, &height, &comp, req_comp);
    free(new_file.data);

    new_image.heigth = (unsigned short)height;
    new_image.width = (unsigned short)width;
    new_image.pixels_data = decoded;
    memcpy(out_image,&new_image,sizeof(Image));
    return 0;
}

void free_image(Image* image){
  stbi_image_free(image->pixels_data);
}
