#version 330

uniform mat4 MVP;
uniform mat4 MV;
uniform vec3 EyePos;
uniform sampler3D vol;

in vec3 eye_to_v;
in vec3 v_pos;

out vec4 frag_colour;

void main(){
     vec3 r_dir = normalize(eye_to_v);
     vec3 r_pos = vec3 (0, 0, 0);

     float val = 0;
     vec3 r_start = v_pos;

     for(int i=0;i<100;i++)
     {
	if (r_start.x > 0.51) break;
	if (r_start.x < -0.51) break;
	if (r_start.y > 0.51) break;
	if (r_start.y < -0.51) break;
	if (r_start.z > 0.51) break;
	if (r_start.z < -0.51) break;

	float p_val = texture(vol, r_start+0.5).r;
	val +=  0.5*texture(vol, r_start+0.5).r;
	r_start += r_dir*0.01;
     }

     ///frag_colour = vec4(texture(vol, r_pos).r,texture(vol, r_pos).r,texture(vol, r_pos).r , 0.4);
     //frag_colour = vec4(eye_to_v*2 , 0.4);
     frag_colour = vec4(val, val, val, 0.4);
}
