#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "utils.h"

#ifdef ANDROID
#include "../../../../../SDK/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/android/asset_manager.h"
AAssetManager* assets_manager;
#endif

#include "numbers.h"
#include <sys/types.h>
struct ResourceDescriptor{
    int32_t descriptor;
    off_t start;
    off_t length;
};

typedef struct file{
  const char *path;
  u32 size_in_bytes;
	u32 bytes_readed; 
 	struct ResourceDescriptor resource_descriptor;
  bool opened;
  void *data;
}File;



int load_file(const char* path, File* output);
void close_file(File* file);


/*Read a specific count of bytes and copy that in the 
 * specified buffer */
int file_read(File* file, char* buffer, int buffer_size);

#endif // !FILE_LOADER_H

