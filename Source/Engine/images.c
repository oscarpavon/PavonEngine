#include "images.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"

#include "file_loader.h"
#include "log.h"

int image_load(const char* path, Image* image){
    File new_file;

    if( load_file(path,&new_file) == -1 ){
        LOG("Image not loaded: %s",path);
        return -1;
    }

    int width, height, comp, req_comp;
    req_comp = 3;

    unsigned char* decoded = stbi_load_from_memory(new_file.data, (int)new_file.size_in_bytes, &width, &height, &comp, req_comp);
    free(new_file.data);

    image->heigth = (unsigned short)height;
    image->width = (unsigned short)width;
    image->pixels_data = decoded;
    return 0;
}

int texture_load(const char* path, Texture* new_texture){
    memset(new_texture,0,sizeof(Texture));
    if(image_load(path,&new_texture->image) == -1){
        new_texture->id = 0;
        return -1;
    }
    load_texture_to_GPU(new_texture); 
}

int load_image_with_format(const char* path, GLint format, Image* out_image){
    File new_file;

    int result = load_file(path,&new_file);
    if(result == -1)
        return -1;

    Image new_image;

    int width, height, comp, req_comp;
    if(format = GL_RGB)
        req_comp = 3;

    if(format = GL_RGBA)
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
