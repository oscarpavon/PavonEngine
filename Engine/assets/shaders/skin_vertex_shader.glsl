#version 100
attribute mediump vec3        	position;
attribute mediump vec3			inColor;
attribute mediump vec2          inUV;
attribute mediump vec4			inJoint0;
attribute mediump vec4			inWeight0;

varying mediump vec2 v_TexCoord;
varying mediump vec3 color;

uniform mat4          MVP;
uniform mat4	joint_matrix[30];
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main(){
	vec4 local_position;
	mat4 skin_matrix;
	
	skin_matrix =
		inWeight0.x * joint_matrix[int(inJoint0.x)] +
		inWeight0.y * joint_matrix[int(inJoint0.y)] +
		inWeight0.z * joint_matrix[int(inJoint0.z)] +
		inWeight0.w * joint_matrix[int(inJoint0.w)];

	mat4 mv = view * model;

	vec4 pos = mv * skin_matrix * vec4(position,1.0);

	gl_Position = projection * pos;
	v_TexCoord = inUV;
	color = inColor;
}
//end