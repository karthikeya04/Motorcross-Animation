#version 330


out vec4 frag_color;

uniform mat4 viewMatrix;

uniform sampler2D texture;
uniform mat3 normalMatrix;
uniform mat4 uModelViewMatrix;

in vec3 normal;
in vec4 eye;
in vec2 tex;
in vec4 color_;
in vec2 tex_flag_;
in vec4 Pos;

vec4 spotLight(vec3 n,vec4 lightPos,vec3 direction,float cutoff,vec4 texImage){
  vec4 diffuse = vec4(0.9, 0.9, 0.9, 1.0);
  vec4 ambient = vec4(0.1, 0, 0, 1.0);
  vec4 specular = vec4(1, 0.5, 0.5, 1.0);
  float shininess = 0.05;
  vec4 spec = vec4(0.0);
  vec4 color = vec4(0.0);

  vec3 lightDir = vec3(lightPos) - vec3(uModelViewMatrix * Pos);
  lightDir = normalize(lightDir);
  float dotProduct = dot(n, lightDir);
  float intensity =  max( dotProduct, 0.0);
  float theta = dot(lightDir,normalize(-direction));

  if(theta > cutoff){
  // if(intensity > 0.0)
  // {
	// vec3 e = normalize(vec3(eye));
	// vec3 h = normalize(lightDir + e );
  //  	float intSpec = max(dot(h,n), 0.0);	
  //       spec = specular * pow(intSpec, shininess);
  // }  
    color +=  max((intensity * diffuse  + spec)*texImage, ambient * texImage); 
  }
  else{
    color += ambient * texImage;
  }
  return color;
}

vec4 pointLight(vec3 n,vec4 lightPos,vec4 texImage){
  vec4 diffuse = vec4(0.9, 0.9, 0.9, 1.0);
  vec4 ambient = vec4(0.1, 0, 0, 1.0);
  vec4 specular = vec4(1, 0.5, 0.5, 1.0);
  float shininess = 0.05;
  vec4 spec = vec4(0.3);

  vec3 lightDir = vec3(viewMatrix * lightPos);  // Transforms with camera
  lightDir = normalize( vec3(lightDir));  
  float dotProduct = dot(n, lightDir);
  float intensity =  max( dotProduct, 0.0);

  // if(intensity > 0.0)
  // {
	// vec3 e = normalize(vec3(eye));
	// vec3 h = normalize(lightDir + e );
  //  	float intSpec = max(dot(h,n), 0.0);	
  //       spec = specular * pow(intSpec, shininess);
  // }  

  vec4 color = (intensity * diffuse  + spec)*texImage; // All
  //color = ambient * texImage;
  return color;
}

void main () 
{
    vec4 texImage;
    if(tex_flag_[0] == 0){
      texImage = color_;
    }
    else{
      texImage = texture2D(texture, tex);
    }
      vec4 ambient = vec4(0.1, 0, 0, 1.0);


    vec4 color = vec4(0.0);
    vec3 n = normalize(vec3(normal));

    // point lights
    vec4 glight1 = vec4(0,10,-10,1);
    vec4 glight2 = vec4(0,10,10,1);

    color += pointLight(n,glight1,texImage);
    color += pointLight(n,glight2,texImage);


    //spotlights
     vec4 lightPos1 = vec4(-1, -10, -1, 1.0);
     //color += spotLight(n,lightPos1,vec3(0,-1,0),0.7,texImage);


    color = max(color, ambient * texImage);
    frag_color = color;

}
