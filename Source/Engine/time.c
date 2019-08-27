#include "time.h"
#include "engine.h"

struct timeval time_measured_01, time_measured_02;
double time_elapsed_time = 0;
struct timespec time_frame_start, time_frame_end;

void time_update_delta(){

}

void time_start(){
    gettimeofday(&time_measured_01, NULL);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_frame_start);     
}

float time_end(){
    gettimeofday(&time_measured_02, NULL);

    //compute
    time_elapsed_time = (time_measured_02.tv_sec - time_measured_01.tv_sec) * 1000.0;
    time_elapsed_time += (time_measured_02.tv_usec - time_measured_01.tv_usec) / 1000.0;

    //convert to seconds
    time_delta = time_elapsed_time /1000;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_frame_end);
        
    struct timespec result = time_diffence(time_frame_start,time_frame_end);
    frame_time = result.tv_nsec / 1000000;  

    return 0;
}