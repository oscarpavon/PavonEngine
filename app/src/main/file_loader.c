
#include <malloc.h>
#include "file_loader.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

void load_file(const char* path, File* output){
#ifdef ANDROID
    AAsset* file =  AAssetManager_open(assets_manager,path,AASSET_MODE_BUFFER);
    if(!file){
        LOGW("Error in loading file: %s",path);
    }
    output->resource_descriptor.descriptor = AAsset_openFileDescriptor(file,&output->resource_descriptor.start,&output->resource_descriptor.length);
    size_t size = AAsset_getLength(file);

    output->data = malloc(size);

    AAsset_read(file,output->data,size);

    output->size_in_bytes = size;

    AAsset_close(file);

#else
    FILE* file = fopen(path,"r");
    if(file == NULL){
        printf("error to load: %s\n", path);
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    void* file_buffer = malloc(file_size);

    fread(file_buffer,sizeof(char), file_size, file);
    output->data = file_buffer;
    output->size_in_bytes = file_size;
    fclose(file);

#endif

}
