#ifndef IMAGES_H
#define IMAGES_H

#include <GLES2/gl2.h>

#include "numbers.h"
typedef struct Image{
  unsigned short int width;
  unsigned short int heigth;
  unsigned char* pixels_data;
}Image;

typedef struct Texture{
    Image image;
    unsigned int id;
}Texture;

int image_load(const char* path, Image* image);
int load_image_with_format(const char* path, GLint format, Image* out_image);
void free_image(Image*);

int texture_load(const char* path, Texture*);

int texture_load_from_memory(Texture* texture,u8 size,void* data);
#endif
