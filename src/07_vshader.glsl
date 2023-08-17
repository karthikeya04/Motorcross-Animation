#version 330

in vec4 vPosition;
in vec2 texCoord;
in vec2 tex_flag;
in vec3 vNormal;
in vec4 vColor;


out vec3 normal;
out vec4 eye;
out vec2 tex;
out vec4 color_;
out vec2 tex_flag_;
out vec4 Pos;

uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;

void main (void) 
{
  Pos = (uModelViewMatrix * vPosition);
  //Pos = vPosition;
  gl_Position = uModelViewMatrix * vPosition;
  eye = - (uModelViewMatrix * gl_Position);
  tex_flag_ = tex_flag;
  normal = (normalMatrix * (vNormal)); 
  if(tex_flag[0] == 1){
    tex = texCoord;
  }
  else{
    color_ = vColor;
  }
}
