#include "images.h"
#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#include "file_loader.h"

Image load_image(const char* path){
    File new_file;

    load_file(path,&new_file);

    Image new_image;

    int width, height, comp, req_comp;
    req_comp = 3;

    unsigned char* decoded = stbi_load_from_memory(new_file.data, (int)new_file.size_in_bytes, &width, &height, &comp, req_comp);
    free(new_file.data);

    new_image.heigth = (unsigned short)height;
    new_image.width = (unsigned short)width;
    new_image.pixels_data = decoded;
    return new_image;
}

void free_image(Image* image){
  stbi_image_free(image->pixels_data);
}
