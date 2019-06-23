//
// Created by pavon on 6/23/19.
//

#include "input.h"

int32_t handle_input(struct android_app* app, AInputEvent* event) {

    int32_t eventType = AInputEvent_getType(event);
    switch(eventType){
        case AINPUT_EVENT_TYPE_MOTION:
            switch(AInputEvent_getSource(event)){
                case AINPUT_SOURCE_TOUCHSCREEN: {

                    int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                    switch (action) {
                        case AMOTION_EVENT_ACTION_DOWN:
                            LOGW("touch");
                            break;
                        case AMOTION_EVENT_ACTION_UP:


                            break;
                        case AMOTION_EVENT_ACTION_MOVE:

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