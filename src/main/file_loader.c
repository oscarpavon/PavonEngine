
#include <malloc.h>
#include "file_loader.h"
#include "utils.h"

void load_file(const char* path, File* output){
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
}
