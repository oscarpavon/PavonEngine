#ifndef IMAGES_H
#define IMAGES_H

#include <GLES2/gl2.h>

typedef struct Image{
  unsigned short int width;
  unsigned short int heigth;
  unsigned char* pixels_data;
}Image;

typedef struct Texture{
    Image image;
    unsigned int id;
}Texture;

Image load_image(const char* path);
int load_image_with_format(const char* path, GLint format, Image* out_image);
void free_image(Image*);

#endif
