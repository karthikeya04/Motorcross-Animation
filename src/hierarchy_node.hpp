#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <utility>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "gl_framework.hpp"
#include "texture.hpp"

struct DOFs
{
	float mnx, mxx;
	float mny, mxy;
	float mnz, mxz;
	int flag; // rotation about only a specific axis
	DOFs()
	{
		mnx = -360;
		mxx = 360;
		mny = -360;
		mxy = 360;
		mnz = -360;
		mxz = 360;
		flag = 0;
	}
	DOFs(float mx, float Mx, float my, float My, float mz, float Mz, int f = 0)
	{
		mnx = mx;
		mxx = Mx;
		mny = my;
		mxy = My;
		mnz = mz;
		mxz = Mz;
		flag = f;
	}
};

struct Camera{
    GLfloat xpos, ypos, zpos;
    GLfloat xrot,yrot,zrot;
	glm::vec3 look_at;
    Camera(){;}
    Camera(GLfloat c_xpos,GLfloat c_ypos,GLfloat c_zpos,GLfloat c_xrot,GLfloat c_yrot,GLfloat c_zrot){
        xpos = c_xpos;
        ypos = c_ypos;
        zpos = c_zpos;
        xrot = c_xrot;
        yrot = c_yrot;
        zrot = c_zrot;
    }
};


namespace csX75
{

	// A simple class that represents a node in the hierarchy tree
	class HNode
	{
		// glm::vec4 * vertices;
		// glm::vec4 * colors;
	public:

		GLfloat tx, ty, tz, rx, ry, rz;

		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;
		std::size_t tex_buffer_size;

		GLuint mytex;

		int tex_idx_ = -1;
		int type = 0;
		GLuint num_vertices;
		GLuint vao, vbo;

		glm::mat4 rotation;
		glm::mat4 translation;

		std::vector<HNode *> children;
		HNode *parent;
		DOFs dof;

		void update_matrices();

		HNode(HNode *);
		HNode(HNode *, GLuint, glm::vec4 *, glm::vec4 *, std::size_t, std::size_t,DOFs dof = DOFs(),glm::vec2* = NULL,std::size_t t_size = 0);
		// HNode (HNode* , glm::vec4*,  glm::vec4*,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);

		void add_child(HNode *);
		void render();
		void change_parameters(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
		void render_tree();
		void inc_rx();
		void inc_ry();
		void inc_rz();
		void dec_rx();
		void dec_ry();
		void dec_rz();
		void inc_tx();
		void inc_ty();
		void inc_tz();
		void dec_tx();
		void dec_ty();
		void dec_tz();
	};

	glm::mat4 *multiply_stack(std::vector<glm::mat4>);
};

#endif