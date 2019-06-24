//
// Created by pavon on 6/23/19.
//

#include "input.h"

static inline void touch_position(float x , float y){
    LOGW("------> X: %f , Y: %f",touch_position_x , touch_position_y);
}

void handle_input_event(struct AInputEvent * event){
    touch_position_x = AMotionEvent_getX(event,0);
    touch_position_y  = AMotionEvent_getY(event,0);
}

void handle_finger_up(){
    touch_position_x = -1;
    touch_position_y = -1;
}

int32_t handle_input(struct android_app* app, AInputEvent* event) {

    int32_t eventType = AInputEvent_getType(event);
    switch(eventType){
        case AINPUT_EVENT_TYPE_MOTION:
            switch(AInputEvent_getSource(event)){
                case AINPUT_SOURCE_TOUCHSCREEN: {

                    int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                    switch (action) {
                        case AMOTION_EVENT_ACTION_DOWN:
                            handle_input_event(event);
                            break;
                        case AMOTION_EVENT_ACTION_UP:
                            handle_finger_up();

                            break;
                        case AMOTION_EVENT_ACTION_MOVE:
                            handle_input_event(event);
                            break;
                    }
                }
                break;
            } // end switch
            break;
        case AINPUT_EVENT_TYPE_KEY:
            // handle key input...
            break;
    } // end switch
    return  -1;
}