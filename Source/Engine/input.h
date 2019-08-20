#ifndef ENGINE_INPUT
#define ENGINE_INPUT

#ifdef ANDROID
    #include "../Platforms/Android/input.h"
#else
    #include "../Editor/input.h"
#endif

#endif