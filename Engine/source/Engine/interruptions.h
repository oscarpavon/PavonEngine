#ifndef ENGINE_INTERRUPTIONS
#define ENGINE_INTERRUPTIONS

#include <signal.h>

static inline void debug_break(){
    raise(SIGINT);
}

#endif