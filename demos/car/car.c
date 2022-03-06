#include "../chess/chess.h"

vec4 color1 = {0,0.2,0,1};
vec4 color2 = {1,0.5,1,1};

vec4 piece_color1 = {0.2,0.2,0.5,1};
vec4 piece_color2 = {0.8,0.5,1,1};

PMesh check_mesh;
PMaterial check_board_mat2;
PMaterial check_board_mat1;


PMaterial piece_mat2;
PMaterial piece_mat1;

PMesh chess_get_mesh(){
  StaticMeshComponent* mesh = get_component_from_element(selected_element,STATIC_MESH_COMPONENT);
  

  Model* original_check_mesh = array_get_pointer(&mesh->models_p,0);
  return original_check_mesh->mesh;
}

void chess_piece_set_pos(vec2 pos){
  
  pe_element_set_position(selected_element,VEC3(pos[0],pos[1],0));

}

void chess_piece_init_scale(){
  float scale = -0.8f;
  pe_element_set_scale(VEC3(scale,scale,scale)) ;
}

void chess_pieces_create(){

  add_element_with_model_path("/home/pavon/chess/rook.glb");
  chess_piece_set_pos(VEC2(4,6)) ;

  chess_piece_init_scale();

  add_element_with_model_path("/home/pavon/chess/pawn.glb");
  chess_piece_set_pos(VEC2(2,0)) ;
  chess_piece_init_scale();

  add_element_with_model_path("/home/pavon/chess/king.glb");
  chess_piece_set_pos(VEC2(3,7)) ;
  chess_piece_init_scale();


  add_element_with_model_path("/home/pavon/chess/queen.glb");
  chess_piece_set_pos(VEC2(5,3)) ;
  chess_piece_init_scale();

}
void chess_move_piece(vec2 pos){
  
  chess_piece_set_pos(pos);

}

void chess_board_create() {

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      new_empty_element();

      add_transform_component_to_selected_element();

      pe_comp_static_mesh_add_to_element();

      pe_element_comp_init();

      StaticMeshComponent *mesh2 =
          get_component_from_element(selected_element, STATIC_MESH_COMPONENT);

        if ((x+y) % 2 == 0) {
          mesh2->material = check_board_mat2;

        } else {

          mesh2->material = check_board_mat1;
        }

      selected_model->mesh = check_mesh;

      pe_element_set_position(selected_element, VEC3(x, y, 0));
    }
  }
}
void chess_input(){
  if(key_released(&input.A)){
    LOG("a pressed\n");
    chess_move_piece(VEC2(1,2))  ;
   }    
  
}
void chess_init_materials(){

  ZERO(check_board_mat1);
  check_board_mat1.shader = shader_standard_color;
  glm_vec4_copy(color1,check_board_mat1.color) ;


  ZERO(check_board_mat2);
  check_board_mat2.shader = shader_standard_color;
  glm_vec4_copy(color2,check_board_mat2.color) ;

  ZERO(piece_mat1);
  piece_mat1.shader = shader_standard_color;
  glm_vec4_copy(piece_color1,piece_mat1.color);
  
  ZERO(piece_mat2);
  piece_mat2.shader = shader_standard_color;
  glm_vec4_copy(piece_color2,piece_mat2.color);
}
void chess_new_empty(){

      new_empty_element();

      add_transform_component_to_selected_element();

      pe_comp_static_mesh_add_to_element();

      pe_element_comp_init();
}
void chess_piece_set_mesh(PMesh mesh){

      StaticMeshComponent *mesh_comp =
          get_component_from_element(selected_element, STATIC_MESH_COMPONENT);

       
      selected_model->mesh = mesh;
}
void chess_create_leaders(){

  add_element_with_model_path("/home/pavon/chess/queen.glb");
  chess_piece_set_pos(VEC2(7,4)) ;
  
  pe_element_set_material(piece_mat1);


  PMesh queen= chess_get_mesh() ;

  chess_new_empty();
  chess_piece_set_mesh(queen); 
  pe_element_set_material(piece_mat2);
  chess_move_piece(VEC2(0,4)) ;


  add_element_with_model_path("/home/pavon/chess/king.glb");
  PMesh king = chess_get_mesh() ;

  chess_move_piece(VEC2(7,3)) ;
  pe_element_set_material(piece_mat1);
  

  chess_new_empty();
  chess_piece_set_mesh(king); 
  pe_element_set_material(piece_mat2);
  chess_move_piece(VEC2(0,3)) ;
}

void chess_create_knight(){

  add_element_with_model_path("/home/pavon/chess/knight.glb");

  chess_piece_set_pos(VEC2(0,1)) ;

  pe_element_set_material(piece_mat2);

  PMesh knight = chess_get_mesh() ;


  chess_new_empty();
  chess_piece_set_mesh(knight); 
  pe_element_set_material(piece_mat2);
  chess_move_piece(VEC2(0,6)) ;
  

  chess_new_empty();
  chess_piece_set_mesh(knight); 
  pe_element_set_material(piece_mat1);
  chess_move_piece(VEC2(7,6)) ;
  
  chess_new_empty();
  chess_piece_set_mesh(knight); 
  pe_element_set_material(piece_mat1);
  chess_move_piece(VEC2(7,1)) ;
}

void chess_create_bishop(){

  add_element_with_model_path("/home/pavon/chess/bishop.glb");

  chess_piece_set_pos(VEC2(0,2)) ;

  pe_element_set_material(piece_mat2);

  PMesh bishop = chess_get_mesh() ;
  

      chess_new_empty();

      chess_piece_set_mesh(bishop); 

      
      pe_element_set_material(piece_mat2);

      
     chess_move_piece(VEC2(0,5)) ;


      chess_new_empty();

      chess_piece_set_mesh(bishop); 

      
      pe_element_set_material(piece_mat1);

      
     chess_move_piece(VEC2(7,5)) ;
      
     chess_new_empty();

      chess_piece_set_mesh(bishop); 

      
      pe_element_set_material(piece_mat1);

      
     chess_move_piece(VEC2(7,2)) ;
}

void chess_create_pawn(){

  add_element_with_model_path("/home/pavon/chess/pawn.glb");

  chess_piece_set_pos(VEC2(1,0)) ;

  pe_element_set_material(piece_mat2);
  StaticMeshComponent* pawn_mesh_comp = get_component_from_element(selected_element,STATIC_MESH_COMPONENT);
  
  Model* model = array_get_pointer(&pawn_mesh_comp->models_p,0);
  PMesh pawn_mesh = model->mesh;
  
  for(int i = 1; i < 8 ; i++){
      chess_new_empty();
      chess_piece_set_mesh(pawn_mesh); 
      pe_element_set_material(piece_mat2);
      
     chess_move_piece(VEC2(1,i)) ;

  }


  for(int i = 0; i < 8 ; i++){
      chess_new_empty();

      chess_piece_set_mesh(pawn_mesh); 

      
      pe_element_set_material(piece_mat1);

      
     chess_move_piece(VEC2(6,i)) ;

  }
}



void chess_create_rooks(){

  add_element_with_model_path("/home/pavon/chess/rook.glb");
  chess_piece_set_pos(VEC2(7,7)) ;
  
  pe_element_set_material(piece_mat1);

  PMesh rook_mesh = chess_get_mesh() ;

  chess_new_empty();
  chess_piece_set_mesh(rook_mesh);
  
  pe_element_set_material(piece_mat1);

  chess_piece_set_pos(VEC2(7,0)) ;
  
  chess_new_empty();
  chess_piece_set_mesh(rook_mesh);
  
  pe_element_set_material(piece_mat2);

  chess_piece_set_pos(VEC2(0,0)) ;
  
  chess_new_empty();
  chess_piece_set_mesh(rook_mesh);
  
  pe_element_set_material(piece_mat2);

  chess_piece_set_pos(VEC2(0,7)) ;
}

void chess_init(){

  camera_init(&main_camera); 
  init_vec3(-7,3.5,3.4, main_camera.position);
  camera_update(&main_camera);
  

  add_element_with_model_path("/home/pavon/chess/check.glb");
 
  chess_init_materials();

  vec3 checkpos = {0,0,29};
  pe_element_set_position(selected_element,checkpos);

  StaticMeshComponent* mesh = get_component_from_element(selected_element,STATIC_MESH_COMPONENT);
  mesh->material = check_board_mat1;
  
  ZERO(check_mesh);

  Model* original_check_mesh = array_get_pointer(&mesh->models_p,0);
  check_mesh = original_check_mesh->mesh;
  
  chess_board_create();
  
  //chess_pieces_create();


  chess_create_pawn();
  chess_create_rooks();
  chess_create_bishop();
  chess_create_knight();
  chess_create_leaders();
}

void chess_loop(){


}

void car_init(){

}

int main(){
    PGame chess;
    ZERO(chess);
    chess.name = "Chess";
    chess.loop = &chess_loop;
    chess.init = &car_init;
    chess.input = &chess_input;
    pe_game_create(&chess);
}
