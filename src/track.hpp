
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include <vector>
#include <set>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"
#include <math.h>
#include "texture.hpp"


/*// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;*/
// Camera position and rotation Parameters


//GLfloat c_xrot=40,c_yrot=-40,c_zrot=0.0;
struct Camera *cam1,*cam2,*cam3,*curr_cam;

GLfloat skybox_x = 14,skybox_y = 14,skybox_z = 14;
GLfloat track_x = -skybox_x/12,track_y = -skybox_y +0.5,track_z = -skybox_z/2.5;
glm::vec3 look_at1 = glm::vec3(track_x,track_y, track_z);
//GLfloat lookat_x = 0,lookat_y = 0,lookat_z = 0;

GLfloat c_xrot1=75,c_yrot1=-5,c_zrot1=0.0;
GLfloat c_xpos1 = 0, c_ypos1 = 0, c_zpos1 = track_z;

GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;

//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool enable_perspective=true;
//Shader program attribs
GLuint vPosition, vColor, texCoord,texFlag,vNormal;
int tex_idx;

const int num_spokes = 12;

float factor3 = 2; // track's
float factor = 0.06 * factor3;  // bike's
float factor2 = 0.1 * factor3; // rider's


//global matrix stack for hierarchical modelling
std::vector<glm::mat4> matrixStack;

csX75::HNode* root_node,*curr_node;
csX75::HNode *wheels[2],*spokes[2*num_spokes],*fender,*engine,*body,*handlebar,*handles;
csX75::HNode *track, *straight1, *straight2, *straight3, *straight4, *straight5, *straight6, *straight7, *straight8, *straight9, *straight10, *straight11, *straight12, *straight13, *straight14, *straight15, *straight16, *straight17, *straight18,*straight19, *straight20, *straight21, *straight22, *straight23, *straight24, *straight25, *straight26, *straight27;
csX75::HNode* torso,*hip,*shoulder,*lleg1,*lleg2,*rleg1,*rleg2,*larm1,*larm2,*rarm1,*rarm2,*neck,*head,*node2,*node3;
csX75::HNode* cam_rider,*cam_helmet;
int mode = 2;
void renderGL();
//-------------------------------------------------------------------------

#endif
