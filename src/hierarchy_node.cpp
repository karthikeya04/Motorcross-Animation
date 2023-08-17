#include "hierarchy_node.hpp"

#include <iostream>

extern GLuint vPosition, vColor, uModelViewMatrix, texCoord, texFlag, vNormal;
;
extern int tex_idx;
extern std::vector<glm::mat4> matrixStack;

float rot_speed = 2; // Keep it a factor of 360
float trans_speed = 0.2;
namespace csX75
{
	HNode::HNode(HNode *a_parent)
	{
		type = 1;
		parent = a_parent;
		parent->add_child(this);

		// initial parameters are set to 0;

		tx = ty = tz = rx = ry = rz = 0;

		update_matrices();
	}
	HNode::HNode(HNode *a_parent, GLuint num_v, glm::vec4 *a_vertices, glm::vec4 *a_colours, std::size_t v_size, std::size_t c_size, DOFs dof, glm::vec2 *a_tex, size_t t_size)
	{

		num_vertices = num_v;
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		tex_buffer_size = t_size;
		// initialize vao and vbo of the object;

		this->dof = dof;
		// Ask GL for a Vertex Attribute Objects (vao)

		glGenVertexArrays(1, &vao);
		// Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers(1, &vbo);

		// bind them
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		if (a_tex)
		{
			this->tex_idx_ = tex_idx;
			if (tex_idx_ == 0)
			{
				mytex = LoadTexture("./src/images/shirt.bmp", 256, 256);
			}
			else if (tex_idx_ == 1)
			{
				mytex = LoadTexture("./src/images/bike.bmp", 256, 256);
			}
			else if (tex_idx_ == 2)
			{
				mytex = LoadTexture("./src/images/track.bmp", 328, 401);
			}
			glm::vec2 tex_flag[num_vertices];
			for (int i = 0; i < num_vertices; i++)
			{
				tex_flag[i] = glm::vec2(1, 1);
			}
			glm::vec4 *v_normals = a_vertices;
			glBufferData(GL_ARRAY_BUFFER, 2 * vertex_buffer_size + tex_buffer_size + sizeof(tex_flag), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices);
			glBufferSubData(GL_ARRAY_BUFFER, vertex_buffer_size, tex_buffer_size, a_tex);
			glBufferSubData(GL_ARRAY_BUFFER, vertex_buffer_size + tex_buffer_size, vertex_buffer_size, v_normals);
			glBufferSubData(GL_ARRAY_BUFFER, 2 * vertex_buffer_size + tex_buffer_size, sizeof(tex_flag), tex_flag);

			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			glEnableVertexAttribArray(texCoord);
			glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

			glEnableVertexAttribArray(vNormal);
			glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + tex_buffer_size));

			glEnableVertexAttribArray(texFlag);
			glVertexAttribPointer(texFlag, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(2 * vertex_buffer_size + tex_buffer_size));
		}
		else
		{
			glm::vec2 tex_flag[num_vertices];
			for (int i = 0; i < num_vertices; i++)
			{
				tex_flag[i] = glm::vec2(0, 0);
			}
			glm::vec4 *v_normals = a_vertices;
			glBufferData(GL_ARRAY_BUFFER, 2 * vertex_buffer_size + color_buffer_size + sizeof(tex_flag), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices);
			glBufferSubData(GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours);
			glBufferSubData(GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, vertex_buffer_size, v_normals);
			glBufferSubData(GL_ARRAY_BUFFER, 2 * vertex_buffer_size + color_buffer_size, sizeof(tex_flag), tex_flag);

			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			glEnableVertexAttribArray(vColor);
			glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

			glEnableVertexAttribArray(vNormal);
			glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + tex_buffer_size));

			glEnableVertexAttribArray(texFlag);
			glVertexAttribPointer(texFlag, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(2 * vertex_buffer_size + tex_buffer_size));
		}
		// setup the vertex array as per the shader

		// set parent

		if (a_parent == NULL)
		{
			parent = NULL;
		}
		else
		{
			parent = a_parent;
			parent->add_child(this);
		}

		// initial parameters are set to 0;

		tx = ty = tz = rx = ry = rz = 0;

		update_matrices();
	}

	void HNode::update_matrices()
	{

		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

		translation = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
	}

	void HNode::add_child(HNode *a_child)
	{
		children.push_back(a_child);
	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz)
	{
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;

		update_matrices();
	}

	void HNode::render()
	{
		if (type != 0)
			return;
		// matrixStack multiply
		glm::mat4 *ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		// for memory
		delete ms_mult;
	}

	void HNode::render_tree()
	{

		matrixStack.push_back(translation);
		matrixStack.push_back(rotation);
		if (tex_idx_ != -1)
		{
			glBindTexture(GL_TEXTURE_2D, mytex);
		}
		render();
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->render_tree();
		}
		matrixStack.pop_back();
		matrixStack.pop_back();
	}

	void HNode::inc_rx()
	{
		if (rx == 360)
		{
			rx = 0;
		}
		if (rx + rot_speed > dof.mxx)
			return;
		rx += rot_speed;
		update_matrices();
	}

	void HNode::inc_ry()
	{
		if (ry == 360)
		{
			ry = 0;
		}
		if (dof.flag)
		{
			if (ry + rot_speed > dof.mxy)
				return;
			ry += rot_speed;
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(ry), glm::vec3(-sin(16 * M_PI / 180), cos(16 * M_PI / 180), 0.0f));
			rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

			translation = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));

			return;
		}
		if (ry + rot_speed > dof.mxy)
			return;
		ry += rot_speed;
		update_matrices();
	}

	void HNode::inc_rz()
	{
		if (rz == 360)
		{
			rz = 0;
		}
		if (rz + rot_speed > dof.mxz)
			return;
		rz += rot_speed;
		update_matrices();
	}

	void HNode::dec_rx()
	{
		if (rx == -360)
		{
			rx = 0;
		}
		if (rx - rot_speed < dof.mnx)
			return;
		rx -= rot_speed;
		update_matrices();
	}

	void HNode::dec_ry()
	{
		if (ry == -360)
		{
			ry = 0;
		}
		if (dof.flag)
		{
			if (ry - rot_speed < dof.mny)
				return;
			ry -= rot_speed;
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(ry), glm::vec3(-sin(16 * M_PI / 180), cos(16 * M_PI / 180), 0.0f));
			rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));

			translation = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
			return;
		}
		if (ry - rot_speed < dof.mny)
			return;
		ry -= rot_speed;
		update_matrices();
	}

	void HNode::dec_rz()
	{
		if (rz == -360)
		{
			rz = 0;
		}
		if (rz - rot_speed < dof.mnz)
			return;
		rz -= rot_speed;
		update_matrices();
	}
	void HNode::inc_tx()
	{
		tx+=trans_speed;
		update_matrices();
	}

	void HNode::inc_ty()
	{
		ty+=trans_speed;
		update_matrices();
	}

	void HNode::inc_tz()
	{
		tz+=trans_speed;
		update_matrices();
	}

	void HNode::dec_tx()
	{
		tx-=trans_speed;
		update_matrices();
	}

	void HNode::dec_ty()
	{
		ty-=trans_speed;
		update_matrices();
	}

	void HNode::dec_tz()
	{
		tz-=trans_speed;
		update_matrices();
	}

	glm::mat4 *multiply_stack(std::vector<glm::mat4> matStack)
	{
		glm::mat4 *mult;
		mult = new glm::mat4(1.0f);

		for (int i = 0; i < matStack.size(); i++)
		{
			*mult = (*mult) * matStack[i];
		}

		return mult;
	}

}