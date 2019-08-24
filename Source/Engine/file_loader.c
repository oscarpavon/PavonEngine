
#include <malloc.h>
#include "file_loader.h"
#include "utils.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void close_file(File* file){
#ifndef ANDROID
        //free((void*)file->path);
#endif
    free(file->data);
}


int load_file(const char* path, File* output){
#ifdef ANDROID
    AAsset* file =  AAssetManager_open(assets_manager,path,AASSET_MODE_BUFFER);
    if(!file){
        LOG("Error in loading file: %s",path);
    }
    output->resource_descriptor.descriptor = AAsset_openFileDescriptor(file,&output->resource_descriptor.start,&output->resource_descriptor.length);
    size_t size = AAsset_getLength(file);

    output->data = malloc(size);

    AAsset_read(file,output->data,size);

    output->size_in_bytes = size;
    output->path = path;
    
    AAsset_close(file);
    return 0;
#else
    
    output->path = path;
    FILE* file = fopen(path,"r");
    if(file == NULL){
        LOG("error to load: %s\n", path);
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind (file);

    void* file_buffer = malloc(file_size);

    fread(file_buffer,1, file_size, file);
    output->data = file_buffer;
    output->size_in_bytes = file_size;
    fclose(file);
    
#endif

}
