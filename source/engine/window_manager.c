#include "windows_manager.h"


void window_manager_update_windows_input(){
	
  	//Draw tab bar 	& draw current tabb 
	for(u8 i = 0; i<engine_windows.count ; i++ ){
		EngineWindow* window = array_get(&engine_windows,i);
		if(!window->initialized)
			   continue;
		
        //The mouse need to stay in the window for window->input call	
		if(window->focus){
			if(window->input)
				window->input();
		}
	}
}



bool pe_wm_should_close(EngineWindow* window){

    return false;
}


void windows_manager_init(){

}

void pe_input_init(){


}
