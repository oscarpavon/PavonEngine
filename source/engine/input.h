#ifndef ENGINE_INPUT
#define ENGINE_INPUT

#ifdef ANDROID
    #include "../Platforms/Android/input.h"
#else
    #include "../editor/input.h"
#endif

#endif
