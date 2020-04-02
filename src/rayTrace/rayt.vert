#version 330

uniform mat4 MVP;
uniform mat4 MV;
uniform vec3 EyePos;

in vec3 vp;
out vec3 eye_to_v;
out vec3 v_pos;

void main() {
     
  gl_Position = MVP * vec4(vp, 1);

  eye_to_v = vp - EyePos;
  v_pos = vp;
};
