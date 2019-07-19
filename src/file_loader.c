
#include <malloc.h>
#include "file_loader.h"
#include "utils.h"
#include "Engine/log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void close_file(File* file){
#ifndef ANDROID
        free((void*)file->path);
#endif
    free(file->data);
}

const char* get_path(const char* path){
    char* editor_path = "../assets/";
    char* buffer = malloc(500);
    memset(buffer,0,500);
    strcat(buffer,editor_path);
    strcat(buffer,path);
    return buffer;
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

#else
    const char* new_path = get_path(path);
    output->path = new_path;
    FILE* file = fopen(new_path,"r");
    if(file == NULL){
        LOG("error to load: %s\n", new_path);
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
