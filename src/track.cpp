/*
  CSX75 Tutorial on Hierarchical Modelling
  Use the arrow keys and PgUp,PgDn,
  keys to make the arms move.
  Use the keys 1,2 and 3 to switch between arms.
  Modified from An Introduction to OpenGL Programming,
  Ed Angel and Dave Shreiner, SIGGRAPH 2013
  Written by -
               Harshavardhan Kode
*/

#include "track.hpp"

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;

glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint uModelViewMatrix;
GLuint viewMatrix;
GLuint normalMatrix;

const int num_vertices = 36;

GLuint vbo_skybox, vao_skybox;

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

//-----------------------------------------------------------------

// Constants
const float pi = M_PI;
const int v_num_wheel = 360 * 360 * 6;
const int v_num_spoke = 360 * 12;
const int v_num_fender = 24;
const int v_num_straight = 36;
const int v_num_body = 36;
const int v_num_straights = 36 * 5;
const int v_num_handlebar = v_num_spoke;
const int v_num_handles = v_num_handlebar;

// Parameters
// <<<<<<< HEAD

glm::vec2 t_coords[4] = {
    glm::vec2(0.0, 0.0),
    glm::vec2(0.0, 1.0),
    glm::vec2(1.0, 0.0),
    glm::vec2(1.0, 1.0)};

float track_width = 0.5 * factor3, track_length = 3.0 / 7 * factor3, track_th = 0.05 * factor3;
float body_w = factor * 4, body_h = factor * 2, body_d = factor * 0.4;
float engine_w = 2, engine_h = 2, engine_d = body_d;
float wheel_r = body_w / 3, wheel_R = wheel_r + body_d;
float spoke_h = wheel_r, spoke_r = wheel_r / 25;
float fender_h = body_h, fender_w = fender_h * 1.4, fender_d = body_d;
float handlebars_h = 2.2 * body_h, handlebars_r = 1.4 * body_d / 2;
float handles_h = 10 * body_d, handles_r = handlebars_r;

// Colors
// =======
// float track_width = 0.5, track_length = 3.0/7, track_th = 0.05;
// float factor = 0.5;
// float body_w = factor*4,body_h = factor*2,body_d = factor*0.4;
// float engine_w = 2,engine_h = 2,engine_d = body_d;
// float wheel_r = body_w/3,wheel_R = wheel_r + body_d;
// float spoke_h = wheel_r,spoke_r = wheel_r/25;
// float fender_h = body_h,fender_w = fender_h*1.4,fender_d = body_d;
// float handlebars_h = 2.2*body_h,handlebars_r = 1.4*body_d/2;
// float handles_h = 10*body_d,handles_r = handlebars_r;

// //Colors
// >>>>>>> 4be4fc7ca434cf6f022eab9df0c6d15b2d4c0cd3
glm::vec4 color_wheel = glm::vec4(0, 0, 1, 1);
glm::vec4 color_spoke = glm::vec4(1, 0, 0, 1);
glm::vec4 color_fender = glm::vec4(0.9, 0, 0, 1);
glm::vec4 color_body = glm::vec4(1, 0, 0, 1);
glm::vec4 color_handlebars = glm::vec4(1, 0, 0, 1);
glm::vec4 color_handles = glm::vec4(0, 1, 0, 1);
glm::vec4 color_body_track = glm::vec4(2, 2, 0, 1);
// glm::vec4 color_straight = glm::vec4(0,1,1,1);

// Vertices
glm::vec4 *cylinder_vertices;

glm::vec4 v_positions_wheel[v_num_wheel];
glm::vec4 v_colors_wheel[v_num_wheel];
glm::vec4 v_normals_wheel[v_num_wheel];

glm::vec4 v_positions_spoke[v_num_spoke];
glm::vec4 v_colors_spoke[v_num_spoke];
glm::vec4 v_normals_spoke[v_num_spoke];

glm::vec4 v_positions_fender[v_num_fender];
glm::vec4 v_colors_fender[v_num_fender];
glm::vec4 v_normals_fender[v_num_fender];

glm::vec4 v_positions_body[v_num_body];
glm::vec4 v_colors_body[v_num_body];
glm::vec4 v_normals_body[v_num_body];
glm::vec2 tex_coords_bike[v_num_body];

glm::vec4 v_positions_straights[27][v_num_straight];
glm::vec4 v_normals_straights[27][v_num_straight];
glm::vec4 v_colors_straights[27][v_num_straight];
glm::vec2 tex_straights[27][v_num_straight];

glm::vec4 v_positions_handlebars[v_num_handlebar];
glm::vec4 v_colors_handlebars[v_num_handlebar];

glm::vec4 v_positions_handles[v_num_handles];
glm::vec4 v_normals_handles[v_num_handles];
glm::vec4 v_colors_handles[v_num_handles];

glm::vec4 v_positions_torso[36];
glm::vec4 v_normals_torso[36];
glm::vec4 v_colors_torso[36];
glm::vec2 tex_coords_rider[36];

glm::vec4 v_positions_hip[36];
glm::vec4 v_normals_hip[36];
glm::vec4 v_colors_hip[36];

glm::vec4 v_positions_head[36];
glm::vec4 v_normals_head[36];
glm::vec4 v_colors_head[36];

glm::vec4 v_positions_lleg1[36];
glm::vec4 v_normals_lleg1[36];
glm::vec4 v_colors_lleg1[36];

glm::vec4 v_positions_lleg2[36];
glm::vec4 v_normals_lleg2[36];
glm::vec4 v_colors_lleg2[36];

glm::vec4 v_positions_rleg1[36];
glm::vec4 v_normals_rleg1[36];
glm::vec4 v_colors_rleg1[36];

glm::vec4 v_positions_rleg2[36];
glm::vec4 v_normals_rleg2[36];
glm::vec4 v_colors_rleg2[36];

glm::vec4 v_positions_shoulder[36];
glm::vec4 v_normals_shoulder[36];
glm::vec4 v_colors_shoulder[36];

glm::vec4 v_positions_larm1[36];
glm::vec4 v_normals_larm1[36];
glm::vec4 v_colors_larm1[36];

glm::vec4 v_positions_larm2[36];
glm::vec4 v_normals_larm2[36];
glm::vec4 v_colors_larm2[36];

glm::vec4 v_positions_rarm1[36];
glm::vec4 v_normals_rarm1[36];
glm::vec4 v_colors_rarm1[36];

glm::vec4 v_positions_rarm2[36];
glm::vec4 v_normals_rarm2[36];
glm::vec4 v_colors_rarm2[36];

glm::vec4 v_positions_neck[36];
glm::vec4 v_normals_neck[36];
glm::vec4 v_colors_neck[36];

void init_cuboid(glm::vec4 *v_positions, glm::vec4 *v_colors, glm::vec4 centre, dimension dim, glm::vec4 color, glm::vec2 *tex_coords = tex_coords_rider)
{
    glm::vec4 positions[8] = {
        glm::vec4(-dim.W / 2.0, -dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(-dim.W / 2.0, dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, -dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(-dim.W / 2.0, -dim.H / 2.0, -dim.D / 2.0, 1.0),
        glm::vec4(-dim.W / 2.0, dim.H / 2.0, -dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, dim.H / 2.0, -dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, -dim.H / 2.0, -dim.D / 2.0, 1.0)};

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            positions[i][j] += centre[j];
        }
    }
    std::vector<std::vector<int>> v = {
        {1, 0, 3, 2},  // back
        {2, 3, 7, 6},  // right
        {3, 0, 4, 7},  // bottom
        {6, 5, 1, 2},  // top
        {4, 5, 6, 7},  // front
        {5, 4, 0, 1}}; // left
    int tri_idx = 0;
    for (int i = 0; i < 6; i++)
    {
        int a = v[i][0], b = v[i][1], c = v[i][2], d = v[i][3];
        v_colors[tri_idx] = color;
        v_positions[tri_idx] = positions[a];
        tex_coords[tri_idx] = t_coords[1];
        tri_idx++;

        v_colors[tri_idx] = color;
        v_positions[tri_idx] = positions[b];
        tex_coords[tri_idx] = t_coords[0];
        tri_idx++;

        v_colors[tri_idx] = color;
        v_positions[tri_idx] = positions[c];
        tex_coords[tri_idx] = t_coords[2];
        tri_idx++;

        v_colors[tri_idx] = color;
        v_positions[tri_idx] = positions[a];
        tex_coords[tri_idx] = t_coords[1];
        tri_idx++;

        v_colors[tri_idx] = color;
        v_positions[tri_idx] = positions[c];
        tex_coords[tri_idx] = t_coords[2];
        tri_idx++;

        v_colors[tri_idx] = color;
        v_positions[tri_idx] = positions[d];
        tex_coords[tri_idx] = t_coords[3];
        tri_idx++;
    }
}

glm::vec4 v_positions_skybox[36];
glm::vec2 tex_skybox[36];

glm::vec2 tex_coords_skybox[6][4] = {
    {glm::vec2(1, 0.34), glm::vec2(1, 0.67), glm::vec2(0.75, 0.34), glm::vec2(0.75, 0.67)},
    {glm::vec2(0.75, 0.34), glm::vec2(0.75, 0.67), glm::vec2(0.5, 0.34), glm::vec2(0.5, 0.67)},
    {glm::vec2(0.25, 0), glm::vec2(0.5, 0), glm::vec2(0.25, 0.34), glm::vec2(0.5, 0.34)},
    {glm::vec2(0.25, 0.67), glm::vec2(0.5, 0.67), glm::vec2(0.25, 1), glm::vec2(0.5, 1)},
    {glm::vec2(0.25, 0.67), glm::vec2(0.25, 0.34), glm::vec2(0.5, 0.67), glm::vec2(0.5, 0.34)},
    {glm::vec2(0.25, 0.34), glm::vec2(0.25, 0.67), glm::vec2(0, 0.34), glm::vec2(0, 0.67)}};

void init_skybox()
{
    dimension dim = dimension(2 * skybox_y, 2 * skybox_x, 2 * skybox_z);
    glm::vec4 centre = glm::vec4(0, 0, 0, 1);
    glm::vec4 positions[8] = {
        glm::vec4(-dim.W / 2.0, -dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(-dim.W / 2.0, dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, -dim.H / 2.0, dim.D / 2.0, 1.0),
        glm::vec4(-dim.W / 2.0, -dim.H / 2.0, -dim.D / 2.0, 1.0),
        glm::vec4(-dim.W / 2.0, dim.H / 2.0, -dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, dim.H / 2.0, -dim.D / 2.0, 1.0),
        glm::vec4(dim.W / 2.0, -dim.H / 2.0, -dim.D / 2.0, 1.0)};
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            positions[i][j] += centre[j];
        }
    }
    std::vector<std::vector<int>> v = {
        {1, 0, 3, 2},  // back
        {2, 3, 7, 6},  // right
        {3, 0, 4, 7},  // bottom
        {6, 5, 1, 2},  // top
        {4, 5, 6, 7},  // front
        {5, 4, 0, 1}}; // left
    int tri_idx = 0;
    glm::vec4 *v_positions = v_positions_skybox;
    glm::vec2 *tex_coords = tex_skybox;
    for (int i = 0; i < 6; i++)
    {
        int a = v[i][0], b = v[i][1], c = v[i][2], d = v[i][3];
        v_positions[tri_idx] = positions[a];
        tex_coords[tri_idx] = tex_coords_skybox[i][1];
        tri_idx++;

        v_positions[tri_idx] = positions[b];
        tex_coords[tri_idx] = tex_coords_skybox[i][0];
        tri_idx++;

        v_positions[tri_idx] = positions[c];
        tex_coords[tri_idx] = tex_coords_skybox[i][2];
        tri_idx++;

        v_positions[tri_idx] = positions[a];
        tex_coords[tri_idx] = tex_coords_skybox[i][1];
        tri_idx++;

        v_positions[tri_idx] = positions[c];
        tex_coords[tri_idx] = tex_coords_skybox[i][2];
        tri_idx++;

        v_positions[tri_idx] = positions[d];
        tex_coords[tri_idx] = tex_coords_skybox[i][3];
        tri_idx++;
    }
}

void init_fender(float h, float b, float w)
{
    glm::vec4 positions[6] = {
        glm::vec4(-b, h, w, 1),
        glm::vec4(0, h, w, 1),
        glm::vec4(0, 0, w, 1),
        glm::vec4(-b, h, 0, 1),
        glm::vec4(0, h, 0, 1),
        glm::vec4(0, 0, 0, 1)};
    for (int i = 0; i < 3; i++)
    {
        v_positions_fender[i] = positions[i];
        v_positions_fender[i + 3] = positions[i + 3];
    }
    std::vector<std::vector<int>> v = {
        {0, 3, 5, 2},
        {1, 2, 5, 4},
        {0, 1, 4, 3}};
    int tri_idx = 6;
    for (int i = 0; i < 3; i++)
    {
        int a = v[i][0], b = v[i][1], c = v[i][2], d = v[i][3];
        v_positions_fender[tri_idx] = positions[a];
        tri_idx++;
        v_positions_fender[tri_idx] = positions[b];
        tri_idx++;
        v_positions_fender[tri_idx] = positions[c];
        tri_idx++;
        v_positions_fender[tri_idx] = positions[a];
        tri_idx++;
        v_positions_fender[tri_idx] = positions[c];
        tri_idx++;
        v_positions_fender[tri_idx] = positions[d];
        tri_idx++;
    }
    for (int i = 0; i < v_num_fender; i++)
    {
        v_colors_fender[i] = color_fender;
    }
}

void init_wheel(float r_, float R_)
{
    glm::vec4 torus_vertices[360 * 360];
    float r = (R_ - r_) / 2, R = r_ + r;
    for (int i = 0; i < 360; i++)
    {
        for (int j = 0; j < 360; j++)
        {
            float v = j * pi / 180, u = i * pi / 180;
            float z = r * sin(v);
            float d = (R + r * cos(v));
            float x = d * cos(u), y = d * sin(u);
            torus_vertices[i * 360 + j] = glm::vec4(x, y, z, 1.0);
        }
    }

    int tri_idx = 0;

    for (int i = 0; i < 360; i++)
    {
        for (int j = 0; j < 360; j++)
        {
            v_positions_wheel[tri_idx] = torus_vertices[i * 360 + j];
            tri_idx++;
            v_positions_wheel[tri_idx] = torus_vertices[i * 360 + (j + 1) % 360];
            tri_idx++;
            v_positions_wheel[tri_idx] = torus_vertices[((i + 1) % 360) * 360 + j];
            tri_idx++;

            v_positions_wheel[tri_idx] = torus_vertices[((i + 1) % 360) * 360 + j];
            tri_idx++;
            v_positions_wheel[tri_idx] = torus_vertices[((i + 1) % 360) * 360 + (j + 1) % 360];
            tri_idx++;
            v_positions_wheel[tri_idx] = torus_vertices[i * 360 + (j + 1) % 360];
            tri_idx++;
        }
    }

    for (int i = 0; i < v_num_wheel; i++)
    {
        v_colors_wheel[i] = color_wheel;
    }
}

void init_cylinder(float h, float r, glm::vec4 *v_positions, glm::vec4 *v_colors, glm::vec4 color, int hor = 0)
{
    cylinder_vertices = new glm::vec4[2 * 360];
    for (int i = 0; i < 360; i++)
    {
        float u = i * pi / 180;
        float x = r * cos(u), z = r * sin(u);
        if (hor)
        {
            cylinder_vertices[i] = glm::vec4(x, z, h / 2, 1.0);
            cylinder_vertices[i + 360] = glm::vec4(x, z, -h / 2, 1.0);
            continue;
        }
        cylinder_vertices[i] = glm::vec4(x, h, z, 1.0);
        cylinder_vertices[i + 360] = glm::vec4(x, 0, z, 1.0);
    }
    glm::vec4 top_centre = glm::vec4(0, h, 0, 1), bottom_centre = glm::vec4(0, 0, 0, 1);
    if (hor)
    {
        top_centre = glm::vec4(0, 0, h / 2, 1);
        bottom_centre = glm::vec4(0, 0, -h / 2, 1);
    }
    int tri_idx = 0;
    for (int i = 0; i < 360; i++)
    {
        v_positions[tri_idx] = cylinder_vertices[i];
        tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i + 1) % 360];
        tri_idx++;
        v_positions[tri_idx] = top_centre;
        tri_idx++;

        v_positions[tri_idx] = cylinder_vertices[i + 360];
        tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i + 1) % 360 + 360];
        tri_idx++;
        v_positions[tri_idx] = bottom_centre;
        tri_idx++;

        v_positions[tri_idx] = cylinder_vertices[i];
        tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i + 1) % 360];
        tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[i + 360];
        tri_idx++;

        v_positions[tri_idx] = cylinder_vertices[i + 360];
        tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i + 1) % 360 + 360];
        tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i + 1) % 360];
        tri_idx++;
    }

    for (int i = 0; i < v_num_spoke; i++)
    {
        v_colors[i] = color;
    }

    delete cylinder_vertices;
}

void init_straight(glm::vec4 *v_positions_straight, glm::vec4 *v_colors_straight, glm::vec2 *tex_straights, float length = track_length)
{

    init_cuboid(v_positions_straight,
                v_colors_straight,
                glm::vec4(0, 0, 0, 1),
                dimension(track_th, length, track_width),
                color_body_track,
                tex_straights);
}

void init_body()
{
    init_cuboid(v_positions_body,
                v_colors_body,
                glm::vec4(0, 0, 0, 1),
                dimension(body_h, body_w, body_d),
                color_body,
                tex_coords_bike);
}
float theight = 2.0 * factor2; // torso's height
float swidth = 4.0 * factor2;  // shoulder's width
float depth = 0.25 * factor2;
float hwidth = 2.0 * factor2;

glm::vec4 skin_color = glm::vec4(1.0, 0.86, 0.67, 1.0);
glm::vec4 shirt_color = glm::vec4(0.6, 0.4, 0.7, 1.0);
glm::vec4 shorts_color = glm::vec4(1.0, 0.0, 0.0, 1.0);

//-----------------------------------------------------------------

void init_track()
{

    tex_idx = 2;
    std::set<int> s = {0, 4, 7, 12, 16, 17, 19, 20, 21, 22, 23, 24, 25};
    for (int i = 0; i < 27; i++)
    {
        if (s.count(i))
        {
            init_straight(v_positions_straights[i], v_colors_straights[i], tex_straights[i], 2 * track_length);
            continue;
        }

        init_straight(v_positions_straights[i], v_colors_straights[i], tex_straights[i]);
    }
    size_t tex_sz = sizeof(tex_straights[0]);
    // <<<<<<< HEAD
    straight1 = new csX75::HNode(NULL, v_num_straight, v_positions_straights[0], v_colors_straights[0], sizeof(v_positions_straights[0]), sizeof(v_colors_straights[0]), DOFs(), tex_straights[0], tex_sz);
    straight1->change_parameters(track_x, track_y, track_z, 0, 0, 0);

    // =======
    //   straight1 = new csX75::HNode(straight13, v_num_straight, v_positions_straights[0], v_colors_straights[0], sizeof(v_positions_straights[0]), sizeof(v_colors_straights[0]));
    //   straight1->change_parameters(0,0,-1,0,0,0);

    //   straight2 = new csX75::HNode(straight1, v_num_straight, v_positions_straights[1], v_colors_straights[1], sizeof(v_positions_straights[1]), sizeof(v_colors_straights[1]));
    //   straight2->change_parameters(1.5*track_length/1.2,track_length/3,0, 0,0,45.0f);

    //   straight3 = new csX75::HNode(straight2, v_num_straight, v_positions_straights[2], v_colors_straights[2], sizeof(v_positions_straights[2]), sizeof(v_colors_straights[2]));
    //   straight3->change_parameters(track_length/1.2,-track_length/3,0, 0,0,-45.0f);

    //   straight4 = new csX75::HNode(straight3, v_num_straight, v_positions_straights[3], v_colors_straights[3], sizeof(v_positions_straights[3]), sizeof(v_colors_straights[3]));
    //   straight4->change_parameters(track_length/1.2,-track_length/3,0, 0,0,-45.0f);
    // >>>>>>> 4be4fc7ca434cf6f022eab9df0c6d15b2d4c0cd3

    straight2 = new csX75::HNode(straight1, v_num_straight, v_positions_straights[1], v_colors_straights[1], sizeof(v_positions_straights[1]), sizeof(v_colors_straights[1]), DOFs(), tex_straights[1], tex_sz);
    straight2->change_parameters(1.5 * track_length / 1.2, track_length / 3, 0, 0, 0, 45.0f);

    straight3 = new csX75::HNode(straight2, v_num_straight, v_positions_straights[2], v_colors_straights[2], sizeof(v_positions_straights[2]), sizeof(v_colors_straights[2]), DOFs(), tex_straights[2], tex_sz);
    straight3->change_parameters(track_length / 1.2, -track_length / 3, 0, 0, 0, -45.0f);

    straight4 = new csX75::HNode(straight3, v_num_straight, v_positions_straights[3], v_colors_straights[3], sizeof(v_positions_straights[3]), sizeof(v_colors_straights[3]), DOFs(), tex_straights[3], tex_sz);
    straight4->change_parameters(track_length / 1.2, -track_length / 3, 0, 0, 0, -45.0f);

    straight5 = new csX75::HNode(straight4, v_num_straight, v_positions_straights[4], v_colors_straights[4], sizeof(v_positions_straights[4]), sizeof(v_colors_straights[4]), DOFs(), tex_straights[4], tex_sz);
    straight5->change_parameters(1.5 * track_length / 1.2, 2 * track_length / 3, 0, 0, 0, 45.0f);

    straight19 = new csX75::HNode(straight5, v_num_straight, v_positions_straights[17], v_colors_straights[17], sizeof(v_positions_straights[17]), sizeof(v_colors_straights[17]), DOFs(), tex_straights[17], tex_sz);
    straight19->change_parameters(2 * track_length - 0.7, 0, 0.4, 0, -90.0f, 0);

    straight6 = new csX75::HNode(straight19, v_num_straight, v_positions_straights[5], v_colors_straights[5], sizeof(v_positions_straights[5]), sizeof(v_colors_straights[5]), DOFs(), tex_straights[5], tex_sz);
    straight6->change_parameters(1.5 * track_length / 1.2, track_length / 3, 0, 0, 0, 45.0f);

    straight7 = new csX75::HNode(straight6, v_num_straight, v_positions_straights[6], v_colors_straights[6], sizeof(v_positions_straights[6]), sizeof(v_colors_straights[6]), DOFs(), tex_straights[6], tex_sz);
    straight7->change_parameters(track_length / 2, -track_length / 2, 0, 0, 0, -90.0f);

    straight8 = new csX75::HNode(straight7, v_num_straight, v_positions_straights[7], v_colors_straights[7], sizeof(v_positions_straights[7]), sizeof(v_colors_straights[7]), DOFs(), tex_straights[7], tex_sz);
    straight8->change_parameters(1.5 * track_length / 1.2, 2 * track_length / 3, 0, 0, 0, 45.0f);

    straight20 = new csX75::HNode(straight8, v_num_straight, v_positions_straights[19], v_colors_straights[19], sizeof(v_positions_straights[19]), sizeof(v_colors_straights[19]), DOFs(), tex_straights[19], tex_sz);
    straight20->change_parameters(2 * track_length - 0.7, 0, 0.4, 0, -90.0f, 0);

    straight9 = new csX75::HNode(straight20, v_num_straight, v_positions_straights[8], v_colors_straights[8], sizeof(v_positions_straights[8]), sizeof(v_colors_straights[8]), DOFs(), tex_straights[8], tex_sz);
    straight9->change_parameters(1.5 * track_length / 1.2, track_length / 3, 0, 0, 0, 45.0f);

    straight10 = new csX75::HNode(straight9, v_num_straight, v_positions_straights[9], v_colors_straights[9], sizeof(v_positions_straights[9]), sizeof(v_colors_straights[9]), DOFs(), tex_straights[9], tex_sz);
    straight10->change_parameters(track_length / 2, -track_length / 2, 0, 0, 0, -90.0f);

    straight11 = new csX75::HNode(straight10, v_num_straight, v_positions_straights[10], v_colors_straights[10], sizeof(v_positions_straights[10]), sizeof(v_colors_straights[10]), DOFs(), tex_straights[10], tex_sz);
    straight11->change_parameters(track_length / 2, track_length / 2, 0, 0, 0, 90.0f);

    straight12 = new csX75::HNode(straight11, v_num_straight, v_positions_straights[11], v_colors_straights[11], sizeof(v_positions_straights[11]), sizeof(v_colors_straights[11]), DOFs(), tex_straights[11], tex_sz);
    straight12->change_parameters(track_length / 2, -track_length / 2, 0, 0, 0, -90.0f);

    // <<<<<<< HEAD
    straight13 = new csX75::HNode(straight12, v_num_straight, v_positions_straights[12], v_colors_straights[12], sizeof(v_positions_straights[12]), sizeof(v_colors_straights[12]), DOFs(), tex_straights[12], tex_sz);
    straight13->change_parameters(1.5 * track_length / 1.2, 2 * track_length / 3, 0, 0, 0, 45.0f);
    // =======
    //   straight21 = new csX75::HNode(straight1, v_num_straight, v_positions_straights[20], v_colors_straights[20], sizeof(v_positions_straights[20]), sizeof(v_colors_straights[20]));
    //   straight21->change_parameters(2*track_length-1,0,-0.4, 0,90.0f,0);
    // >>>>>>> 4be4fc7ca434cf6f022eab9df0c6d15b2d4c0cd3

    straight21 = new csX75::HNode(straight1, v_num_straight, v_positions_straights[20], v_colors_straights[20], sizeof(v_positions_straights[20]), sizeof(v_colors_straights[20]), DOFs(), tex_straights[20], tex_sz);
    straight21->change_parameters(2 * track_length - 2, 0, -1, 0, 90.0f, 0);

    straight14 = new csX75::HNode(straight21, v_num_straight, v_positions_straights[13], v_colors_straights[13], sizeof(v_positions_straights[13]), sizeof(v_colors_straights[13]), DOFs(), tex_straights[13], tex_sz);
    straight14->change_parameters(1.5 * track_length / 1.2, track_length / 3, 0, 0, 0, 45.0f);

    straight15 = new csX75::HNode(straight14, v_num_straight, v_positions_straights[14], v_colors_straights[14], sizeof(v_positions_straights[14]), sizeof(v_colors_straights[14]), DOFs(), tex_straights[14], tex_sz);
    straight15->change_parameters(track_length / 1.2, -track_length / 3, 0, 0, 0, -45.0f);

    straight16 = new csX75::HNode(straight15, v_num_straight, v_positions_straights[15], v_colors_straights[15], sizeof(v_positions_straights[15]), sizeof(v_colors_straights[15]), DOFs(), tex_straights[15], tex_sz);
    straight16->change_parameters(track_length / 1.2, track_length / 3, 0, 0, 0, 45.0f);

    straight17 = new csX75::HNode(straight16, v_num_straight, v_positions_straights[16], v_colors_straights[16], sizeof(v_positions_straights[16]), sizeof(v_colors_straights[16]), DOFs(), tex_straights[16], tex_sz);
    straight17->change_parameters(track_length / 2, -2 * track_length / 2, 0, 0, 0, -90.0f);

    straight18 = new csX75::HNode(straight17, v_num_straight, v_positions_straights[17], v_colors_straights[17], sizeof(v_positions_straights[17]), sizeof(v_colors_straights[17]), DOFs(), tex_straights[17], tex_sz);
    straight18->change_parameters(2 * track_length / 1.2, 2 * track_length / 3, 0, 0, 0, 45.0f);

    straight22 = new csX75::HNode(straight18, v_num_straight, v_positions_straights[21], v_colors_straights[21], sizeof(v_positions_straights[21]), sizeof(v_colors_straights[21]), DOFs(), tex_straights[21], tex_sz);
    straight22->change_parameters(2 * track_length - 0.7, 0, -0.4, 0, 90.0f, 0);

    straight23 = new csX75::HNode(straight22, v_num_straight, v_positions_straights[22], v_colors_straights[22], sizeof(v_positions_straights[22]), sizeof(v_colors_straights[22]), DOFs(), tex_straights[22], tex_sz);
    straight23->change_parameters(2 * track_length / 1.2, 2 * track_length / 3, 0, 0, 0, 45.0f);

    straight24 = new csX75::HNode(straight23, v_num_straight, v_positions_straights[24], v_colors_straights[24], sizeof(v_positions_straights[24]), sizeof(v_colors_straights[24]), DOFs(), tex_straights[24], tex_sz);
    straight24->change_parameters(2 * track_length / 2, -2 * track_length / 2, 0, 0, 0, -90.0f);

    straight25 = new csX75::HNode(straight24, v_num_straight, v_positions_straights[24], v_colors_straights[24], sizeof(v_positions_straights[24]), sizeof(v_colors_straights[24]), DOFs(), tex_straights[24], tex_sz);
    straight25->change_parameters(2 * track_length / 1.2, 2 * track_length / 3, 0, 0, 0, 45.0f);

    // straight26 = new csX75::HNode(straight17, v_num_straight, v_positions_straights[17], v_colors_straights[17], sizeof(v_positions_straights[17]), sizeof(v_colors_straights[17]));
    // straight18->change_parameters(2*track_length/1.2,2*track_length/3,0, 0,0,45.0f);

    // straight22 = new csX75::HNode(straight17, v_num_straight, v_positions_straights[17], v_colors_straights[17], sizeof(v_positions_straights[17]), sizeof(v_colors_straights[17]));
    // straight18->change_parameters(2*track_length/1.2,2*track_length/3,0, 0,0,45.0f);

    // straight22 = new csX75::HNode(straight17, v_num_straight, v_positions_straights[17], v_colors_straights[17], sizeof(v_positions_straights[17]), sizeof(v_colors_straights[17]));
    // straight18->change_parameters(2*track_length/1.2,2*track_length/3,0, 0,0,45.0f);

    // root_node = straight1;
    // curr_node = straight1;
}

void init_bike()
{

    tex_idx = 1;

    init_wheel(wheel_r, wheel_R);
    init_fender(fender_h, fender_w, fender_d);
    init_body();
    // note that the buffers are initialized in the respective constructors
    body = new csX75::HNode(straight1, v_num_body, v_positions_body, v_colors_body, sizeof(v_positions_body), sizeof(v_colors_body), DOFs(), tex_coords_bike, sizeof(tex_coords_bike));
    body->change_parameters(0.3 * body_w, 0.8 * body_h + wheel_R, 5*track_length, 0, 0, 0);
    fender = new csX75::HNode(body, v_num_fender, v_positions_fender, v_colors_fender, sizeof(v_positions_fender), sizeof(v_colors_fender));
    fender->change_parameters(-body_w / 2, -body_h / 2, -body_d / 2, 0, 0, 0);

    init_cylinder(handlebars_h, handlebars_r, v_positions_handlebars, v_colors_handlebars, color_handlebars);
    handlebar = new csX75::HNode(body, v_num_wheel, v_positions_handlebars, v_colors_handlebars, sizeof(v_positions_handlebars), sizeof(v_colors_handlebars), DOFs(0, 0, -360, 360, 0, 0, 1));
    handlebar->change_parameters(body_w / 2 + fender_w / 5, -0.8 * body_h, 0, 0, 0, 16);

    init_cylinder(handles_h, handles_r, v_positions_handles, v_colors_handles, color_handles, 1);
    handles = new csX75::HNode(handlebar, v_num_wheel, v_positions_handles, v_colors_handles, sizeof(v_positions_handles), sizeof(v_colors_handles));
    handles->change_parameters(-0.7 * handlebars_r, handlebars_h * 0.9, 0, 0, 0, 0);

    init_cylinder(spoke_h, spoke_r, v_positions_spoke, v_colors_spoke, color_spoke);

    wheels[0] = new csX75::HNode(body, v_num_wheel, v_positions_wheel, v_colors_wheel, sizeof(v_positions_wheel), sizeof(v_colors_wheel), DOFs(0, 0, 0, 0, -360, 360));
    wheels[0]->change_parameters(-body_w / 2 - fender_w / 2, -0.8 * body_h, 0, 0, 0, 0);

    wheels[1] = new csX75::HNode(handlebar, v_num_wheel, v_positions_wheel, v_colors_wheel, sizeof(v_positions_wheel), sizeof(v_colors_wheel), DOFs(0, 0, 0, 0, -360, 360));
    wheels[1]->change_parameters(0, 0, 0, 0, 0, 0);
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < num_spokes; i++)
        {
            spokes[i + j * num_spokes] = new csX75::HNode(wheels[j], v_num_spoke, v_positions_spoke, v_colors_spoke, sizeof(v_positions_spoke), sizeof(v_colors_spoke));
            float u = i * 360 / (num_spokes);
            spokes[i + j * num_spokes]->change_parameters(0, 0, 0, 0, 0, u);
        }
    }

    // root_node = body;
    // curr_node = body;
}

// float theight = 2.0; // torso's height
// float swidth = 4.0;  // shoulder's width
// float depth = 0.25;
// float hwidth = 2.0;

// glm::vec4 skin_color = glm::vec4(1.0, 0.86, 0.67, 1.0);
// glm::vec4 shirt_color = glm::vec4(0.6, 0.4, 0.7, 1.0);
// glm::vec4 shorts_color = glm::vec4(1.0, 0.0, 0.0, 1.0);

void init_torso()
{

    init_cuboid(v_positions_torso,
                v_colors_torso,
                glm::vec4(0.0, theight / 2.0, 0.0, 1.0),
                dimension(theight, theight / 2.0, depth),
                shirt_color,
                tex_coords_rider);
}

void init_hip()
{

    init_cuboid(v_positions_hip,
                v_colors_hip,
                glm::vec4(0.0, -theight / 24.0, 0.0, 1.0),
                dimension(theight / 12.0, theight / 2, 2 * depth),
                glm::vec4(1.0, 0.0, 0.0, 1.0));
}

void init_shoulder()
{

    init_cuboid(v_positions_shoulder,
                v_colors_shoulder,
                glm::vec4(0.0, theight / 24.0, 0.0, 1.0),
                dimension(theight / 12.0, theight / 1.2, depth),
                shirt_color);
}

void init_llegs()
{
    init_cuboid(v_positions_lleg1,
                v_colors_lleg1,
                glm::vec4(0, -theight / 4.0, 0.0, 1.0),
                dimension(theight / 2, theight / 6, depth),
                shorts_color);
    init_cuboid(v_positions_lleg2,
                v_colors_lleg2,
                glm::vec4(0, -theight / 4.0, 0.0, 1.0),
                dimension(theight / 2, theight / 6, depth),
                skin_color);
}

void init_rlegs()
{
    init_cuboid(v_positions_rleg1,
                v_colors_rleg1,
                glm::vec4(0, -theight / 4.0, 0.0, 1.0),
                dimension(theight / 2, theight / 6, depth),
                shorts_color);
    init_cuboid(v_positions_rleg2,
                v_colors_rleg2,
                glm::vec4(0, -theight / 4.0, 0.0, 1.0),
                dimension(theight / 2, theight / 6, depth),
                skin_color);
}

void init_larms()
{
    init_cuboid(v_positions_larm1,
                v_colors_larm1,
                glm::vec4(0, -theight / 6.0, 0.0, 1.0),
                dimension(theight / 3.0, theight / 6.0, depth),
                shirt_color);
    init_cuboid(v_positions_larm2,
                v_colors_larm2,
                glm::vec4(0, -theight / 3.0, 0.0, 1.0),
                dimension(theight / 1.5, theight / 6.0, depth),
                skin_color);
}

void init_rarms()
{
    init_cuboid(v_positions_rarm1,
                v_colors_rarm1,
                glm::vec4(0, -theight / 6.0, 0.0, 1.0),
                dimension(theight / 3.0, theight / 6.0, depth),
                shirt_color);
    init_cuboid(v_positions_rarm2,
                v_colors_rarm2,
                glm::vec4(0, -theight / 3.0, 0.0, 1.0),
                dimension(theight / 1.5, theight / 6.0, depth),
                skin_color);
}

void init_neck()
{
    init_cuboid(v_positions_neck,
                v_colors_neck,
                glm::vec4(0.0, theight / 20, 0.0, 1.0),
                dimension(theight / 10.0, theight / 8.0, depth),
                glm::vec4(0.0, 0.9, 0.0, 1.0));
}

void init_head()
{
    init_cuboid(v_positions_head,
                v_colors_head,
                glm::vec4(0.0, theight / 6, 0.0, 1.0),
                dimension(theight / 3, theight / 3.0, depth),
                skin_color);
}
void init_rider()
{
    tex_idx = 0;
    init_torso();
    init_hip();
    init_shoulder();
    init_llegs();
    init_rlegs();
    init_larms();
    init_rarms();
    init_neck();
    init_head();

    std::size_t pos_size = sizeof(v_positions_hip);
    std::size_t col_size = sizeof(v_colors_hip);

    // note that the buffers are initialized in the respective constructors

    hip = new csX75::HNode(straight1, num_vertices, v_positions_hip, v_colors_hip, pos_size, col_size);
    hip->change_parameters(0, theight, 5*track_length, 0, 90, 0);
    cam_rider = new csX75::HNode(hip);
    cam_rider->change_parameters(4,1,1,0,0,0);
    torso = new csX75::HNode(hip, num_vertices, v_positions_torso, v_colors_torso, pos_size, col_size, DOFs(), tex_coords_rider, sizeof(tex_coords_rider));

    shoulder = new csX75::HNode(torso, num_vertices, v_positions_shoulder, v_colors_shoulder, pos_size, col_size);
    shoulder->change_parameters(0, theight, 0, 0, 0, 0);

    lleg1 = new csX75::HNode(hip, num_vertices, v_positions_lleg1, v_colors_lleg1, pos_size, col_size, DOFs(-90, 90, -90, 90, 0, 0));
    lleg1->change_parameters(-theight / 4.0 + theight / 12.0, -theight / 12, 0, 0, 0, 0);

    lleg2 = new csX75::HNode(lleg1, num_vertices, v_positions_lleg2, v_colors_lleg2, pos_size, col_size, DOFs(0, 180, 0, 0, 0, 0));
    lleg2->change_parameters(0, -theight / 2, 0, 0, 0, 0);

    rleg1 = new csX75::HNode(hip, num_vertices, v_positions_rleg1, v_colors_rleg1, pos_size, col_size, DOFs(-90, 90, -90, 90, 0, 0));
    rleg1->change_parameters(theight / 4.0 - theight / 12.0, -theight / 12, 0, 0, 0, 0);

    rleg2 = new csX75::HNode(rleg1, num_vertices, v_positions_rleg2, v_colors_rleg2, pos_size, col_size, DOFs(0, 180, 0, 0, 0, 0));
    rleg2->change_parameters(0, -theight / 2, 0, 0, 0, 0);

    larm1 = new csX75::HNode(shoulder, num_vertices, v_positions_larm1, v_colors_larm1, pos_size, col_size, DOFs(-180, 90, -90, 90, 0, 0));
    larm1->change_parameters(-theight / 2.4 + theight / 12.0, 0, 0, 0, 0, 0);

    larm2 = new csX75::HNode(larm1, num_vertices, v_positions_larm2, v_colors_larm2, pos_size, col_size, DOFs(-180, 0, 0, 0, 0, 0));
    larm2->change_parameters(0, -theight / 3, 0, 0, 0, 0);

    rarm1 = new csX75::HNode(shoulder, num_vertices, v_positions_rarm1, v_colors_rarm1, pos_size, col_size, DOFs(-180, 90, -90, 90, 0, 0));
    rarm1->change_parameters(theight / 2.4 - theight / 12.0, 0, 0, 0, 0, 0);

    rarm2 = new csX75::HNode(rarm1, num_vertices, v_positions_rarm2, v_colors_rarm2, pos_size, col_size, DOFs(-180, 0, 0, 0, 0, 0));
    rarm2->change_parameters(0, -theight / 3, 0, 0, 0, 0);

    neck = new csX75::HNode(shoulder, num_vertices, v_positions_neck, v_colors_neck, pos_size, col_size);
    neck->change_parameters(0, theight / 12, 0, 0, 0, 0);

    head = new csX75::HNode(neck, num_vertices, v_positions_head, v_colors_head, pos_size, col_size);
    head->change_parameters(0, theight / 10, 0, 0, 0, 0);
    cam_helmet = new csX75::HNode(head);
}

GLuint skybox_tex;
void initBuffersGL(void)
{

    // Load shaders and use the resulting shader program
    std::string vertex_shader_file("./src/07_vshader.glsl");
    std::string fragment_shader_file("./src/07_fshader.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    shaderProgram = csX75::CreateProgramGL(shaderList);
    glUseProgram(shaderProgram);

    // getting the attributes from the shader program
    vPosition = glGetAttribLocation(shaderProgram, "vPosition");
    vColor = glGetAttribLocation(shaderProgram, "vColor");
    texCoord = glGetAttribLocation(shaderProgram, "texCoord");
    texFlag = glGetAttribLocation(shaderProgram, "tex_flag");
    vNormal = glGetAttribLocation(shaderProgram, "vNormal");

    uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
    normalMatrix = glGetUniformLocation(shaderProgram, "normalMatrix");
    viewMatrix = glGetUniformLocation(shaderProgram, "viewMatrix");
    // Creating the hierarchy:
    // We are using the original colorcube function to generate the vertices of the cuboid
    skybox_tex = LoadTexture("./src/images/skybox2.bmp", 1024, 768);

    init_skybox();

    glm::vec2 tex_flag[36];
    for (int i = 0; i < 36; i++)
    {
        tex_flag[i] = glm::vec2(1, 1);
    }
    glGenVertexArrays(1, &vao_skybox);
    glGenBuffers(1, &vbo_skybox);
    glBindVertexArray(vao_skybox);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_skybox);
    
    glm::vec4 *v_normals_skybox = v_positions_skybox;
    glBufferData(GL_ARRAY_BUFFER, 2*sizeof(v_positions_skybox) + sizeof(tex_skybox) + sizeof(tex_flag), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions_skybox), v_positions_skybox);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions_skybox), sizeof(tex_skybox), tex_skybox);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions_skybox) + sizeof(tex_skybox), sizeof(v_positions_skybox), v_normals_skybox);
    glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(v_positions_skybox) + sizeof(tex_skybox), sizeof(tex_flag), tex_flag);

    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(texCoord);
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions_skybox)));
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions_skybox) + sizeof(tex_skybox)));
    glEnableVertexAttribArray(texFlag);
    glVertexAttribPointer(texFlag, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(2 * sizeof(v_positions_skybox) + sizeof(tex_skybox)));
    init_track();
    init_bike();
    init_rider();
    curr_node = straight1;
    root_node = straight1;
}

void update_cam3(){
    glm::vec4 pos = (straight1->translation * straight1->rotation * 
                    hip->translation * hip->rotation * 
                    torso->translation * torso->rotation * 
                    shoulder->translation * shoulder->rotation * 
                    neck->translation *neck->rotation * 
                    head->translation * head->rotation) * glm::vec4(0,0,0,1);
    cam3->xpos = pos[0]+0.01 ; cam3->ypos = pos[1]; cam3->zpos = pos[2] ;    
    //std::cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<"\n";
    //cam3->look_at = glm::vec3(10,-track_y,0);
    cam3->look_at = glm::vec3(pos[0]+0.5,pos[1],pos[2]);



}

void update_cam2(){
    glm::vec3 track_centre = glm::vec3(track_x,track_y,track_z);
    glm::vec4 pos = (straight1->translation * straight1->rotation * 
                    hip->translation * hip->rotation) * glm::vec4(0,0,0,1);
    cam2->xpos = pos[0] ; cam2->ypos = pos[1]; cam2->zpos = pos[2];    
    //std::cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<"\n";
    // cam2->look_at = glm::vec3(track_centre[0],track_centre[1]+theight,track_centre[2] + 5*track_length);
    cam2->look_at = glm::vec3(pos[0]+0.5,pos[1],pos[2]);
}

void renderGL(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    matrixStack.clear();

    // Creating the lookat and the up vectors for the camera
    update_cam2();
    update_cam3();

    GLfloat c_xpos = curr_cam->xpos,c_ypos = curr_cam->ypos,c_zpos = curr_cam->zpos;
    GLfloat c_xrot = curr_cam->xrot,c_yrot = curr_cam->yrot,c_zrot = curr_cam->zrot;
    //std::cout<<c_xrot<<" "<<c_yrot<<" "<<c_zrot<<"\n";
    glm::mat4 Tr1 = glm::translate(glm::mat4(1.0f), glm::vec3(-c_xpos, -c_ypos, -c_zpos));
    glm::mat4 Tr2 = glm::translate(glm::mat4(1.0f), glm::vec3(c_xpos, c_ypos, c_zpos));
    c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f, 0.0f, 0.0f));
    c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f, 1.0f, 0.0f));
    c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * Tr1 * c_rotation_matrix * Tr2;
    glm::vec4 c_up = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * Tr1 * c_rotation_matrix * Tr2;
    // Creating the lookat matrix
    lookat_matrix = glm::lookAt(glm::vec3(c_pos), curr_cam->look_at, glm::vec3(c_up));

    // creating the projection matrix
    if (enable_perspective)
        // projection_matrix = glm::frustum(-10.0, 10.0, -10.0, 10.0, 10.0, 10.0);
        projection_matrix = glm::perspective(glm::radians(80.0), 1.0, 1.0, 50.0);
    else
        projection_matrix = glm::ortho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);

    view_matrix = projection_matrix * lookat_matrix;
    
    glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));

    // rendering skybox

    modelview_matrix = view_matrix * model_matrix;
    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
    normal_matrix = glm::transpose(glm::inverse(glm::mat3(modelview_matrix)));
    glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glBindTexture(GL_TEXTURE_2D, skybox_tex);

    glBindVertexArray(vao_skybox);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // rendering track

    matrixStack.push_back(view_matrix);

    root_node->render_tree();
}

int main(int argc, char **argv)
{
    cam1 = new Camera(c_xpos1,c_ypos1,c_zpos1,c_xrot1,c_yrot1,c_zrot1);
    cam2 = new Camera();
    cam3 = new Camera();
    curr_cam = cam1;
    cam1->look_at = look_at1;
    cam2->xrot = -5; cam2->yrot = 0; cam2->zrot = -5;
    //! The pointer to the GLFW window
    GLFWwindow *window;

    //! Setting up the GLFW Error callback
    glfwSetErrorCallback(csX75::error_callback);

    //! Initialize GLFW
    if (!glfwInit())
        return -1;

    // We want OpenGL 4.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // This is for MacOSX - can be omitted otherwise
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // We don't want the old OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //! Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1024, 1024, "CS475/CS675 Tutorial 7: Hierarchical Modelling", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //! Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    // Turn this on to get Shader based OpenGL
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Problem: glewInit failed, something is seriously wrong.
        std::cerr << "GLEW Init Failed : %s" << std::endl;
    }

    // Print and see what context got enabled
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Keyboard Callback
    glfwSetKeyCallback(window, csX75::key_callback);
    // Framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Initialize GL state
    csX75::initGL();
    initBuffersGL();

    // Loop until the user closes the window
    while (glfwWindowShouldClose(window) == 0)
    {

        // Render here
        renderGL();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//-------------------------------------------------------------------------