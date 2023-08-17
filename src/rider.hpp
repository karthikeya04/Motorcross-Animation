
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"
#include "texture.hpp"


struct dimension
{
    float H;
    float W;
    float D;
    dimension(float h, float w, float d)
    {
        H = h;
        W = w;
        D = d;
    }
};

/*// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;*/
// Camera position and rotation Parameters
GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 2.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot = 0.0, c_yrot = 0.0, c_zrot = 0.0;

// Running variable to toggle culling on/off
bool enable_culling = true;
// Running variable to toggle wireframe/solid modelling
bool solid = true;
// Enable/Disable perspective view
bool enable_perspective = false;
// Shader program attribs
GLuint vPosition, vColor, texCoord,texFlag;
int tex_idx = 0;
const int num_spokes = 12;

// global matrix stack for hierarchical modelling
std::vector<glm::mat4> matrixStack;

csX75::HNode *root_node;
csX75::HNode *torso, *hip, *shoulder, *lleg1, *lleg2, *rleg1, *rleg2, *larm1, *larm2, *rarm1, *rarm2, *neck, *head, *node2, *node3, *curr_node;
csX75::HNode *wheels[2],*spokes[2*num_spokes],*fender,*engine,*body,*handlebar,*handles;

int mode = 0;
//-------------------------------------------------------------------------

#endif
