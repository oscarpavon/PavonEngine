cmake_minimum_required(VERSION 2.6)

project(FreeType2)

set(FT_FOLDER /home/pavon/sources/freetype2/)
# First, compiler definitions for building the library
add_definitions(-DFT2_BUILD_LIBRARY)
#add_definitions("-DFT_CONFIG_MODULES_H=<ftmodule.h>")

# Specify library include directories
include_directories("${FT_FOLDER}/builds/ansi")
include_directories("${FT_FOLDER}/include")
include_directories("${FT_FOLDER}/include/freetype")
#include_directories("${PROJECT_SOURCE_DIR}/include/freetype/config")

# For the auto-generated ftmodule.h file
include_directories("${PROJECT_BINARY_DIR}/include")

include_directories("${PROJECT_SOURCE_DIR}/objs")

#file(GLOB BASE_SRCS "src/base/*.c")

set(BASE_SRCS
${FT_FOLDER}/src/base/ftbase.c
${FT_FOLDER}/src/autofit/autofit.c
${FT_FOLDER}/src/base/ftbbox.c
${FT_FOLDER}/src/base/ftbdf.c
${FT_FOLDER}/src/base/ftbitmap.c
${FT_FOLDER}/src/base/ftcid.c
${FT_FOLDER}/src/base/ftfstype.c
${FT_FOLDER}/src/base/ftgasp.c
${FT_FOLDER}/src/base/ftglyph.c
${FT_FOLDER}/src/base/ftgxval.c
${FT_FOLDER}/src/base/ftinit.c
${FT_FOLDER}/src/base/ftmm.c
${FT_FOLDER}/src/base/ftotval.c
${FT_FOLDER}/src/base/ftpatent.c
${FT_FOLDER}/src/base/ftpfr.c
${FT_FOLDER}/src/base/ftstroke.c
${FT_FOLDER}/src/base/ftsynth.c
${FT_FOLDER}/src/base/ftsystem.c
${FT_FOLDER}/src/base/fttype1.c
${FT_FOLDER}/src/base/ftwinfnt.c
${FT_FOLDER}/src/bdf/bdf.c
${FT_FOLDER}/src/bzip2/ftbzip2.c
${FT_FOLDER}/src/cache/ftcache.c
${FT_FOLDER}/src/cff/cff.c
${FT_FOLDER}/src/cid/type1cid.c
${FT_FOLDER}/src/gzip/ftgzip.c
${FT_FOLDER}/src/lzw/ftlzw.c
${FT_FOLDER}/src/pcf/pcf.c
${FT_FOLDER}/src/pfr/pfr.c
${FT_FOLDER}/src/psaux/psaux.c
${FT_FOLDER}/src/pshinter/pshinter.c
${FT_FOLDER}/src/psnames/psnames.c
${FT_FOLDER}/src/raster/raster.c
${FT_FOLDER}/src/sfnt/sfnt.c
${FT_FOLDER}/src/smooth/smooth.c
${FT_FOLDER}/src/truetype/truetype.c
${FT_FOLDER}/src/type1/type1.c
${FT_FOLDER}/src/type42/type42.c
${FT_FOLDER}/src/winfonts/winfnt.c
    
)

include_directories("${FT_FOLDER}/src/truetype")
include_directories("${FT_FOLDER}/src/sfnt")
include_directories("${FT_FOLDER}/src/autofit")
include_directories("${FT_FOLDER}/src/smooth")
include_directories("${FT_FOLDER}/src/raster")
include_directories("${FT_FOLDER}/src/psaux")
include_directories("${FT_FOLDER}/src/psnames")

add_library(freetype STATIC ${BASE_SRCS})

set(FREETYPE_LIBRARY freetype CACHE STRING "The FreeType library name")
set(FREETYPE_FOUND TRUE CACHE BOOL "Whether freetype has been found or not")
set(FREETYPE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/include/freetype CACHE STRING "FreeType include
    directories")

    set(FREETYPE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/include/freetype CACHE STRING "FreeType include
    directories")