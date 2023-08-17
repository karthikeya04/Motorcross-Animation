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

#include "rider.hpp"


GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;

glm::mat4 modelview_matrix;

GLuint uModelViewMatrix;
const int num_vertices = 36;

//-----------------------------------------------------------------

// Eight vertices in homogenous coordinates
// elongated cuboid , basic arm in the hierarchy;

// RGBA colors
glm::vec2 t_coords[4] = {
  glm::vec2( 0.0, 0.0),
  glm::vec2( 0.0, 1.0),
  glm::vec2( 1.0, 0.0),
  glm::vec2( 1.0, 1.0)
};


glm::vec4 colors[8] = {
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    glm::vec4(1.0, 0.0, 0.0, 1.0),
    glm::vec4(1.0, 1.0, 0.0, 1.0),
    glm::vec4(0.0, 1.0, 0.0, 1.0),
    glm::vec4(0.0, 0.0, 1.0, 1.0),
    glm::vec4(1.0, 0.0, 1.0, 1.0),
    glm::vec4(1.0, 1.0, 1.0, 1.0),
    glm::vec4(0.0, 1.0, 1.0, 1.0)};


glm::vec4 v_positions_torso[36];
glm::vec4 v_colors_torso[36];
glm::vec2 tex_coords_rider[36];

glm::vec4 v_positions_hip[36];
glm::vec4 v_colors_hip[36];

glm::vec4 v_positions_head[36];
glm::vec4 v_colors_head[36];

glm::vec4 v_positions_lleg1[36];
glm::vec4 v_colors_lleg1[36];

glm::vec4 v_positions_lleg2[36];
glm::vec4 v_colors_lleg2[36];

glm::vec4 v_positions_rleg1[36];
glm::vec4 v_colors_rleg1[36];

glm::vec4 v_positions_rleg2[36];
glm::vec4 v_colors_rleg2[36];

glm::vec4 v_positions_shoulder[36];
glm::vec4 v_colors_shoulder[36];

glm::vec4 v_positions_larm1[36];
glm::vec4 v_colors_larm1[36];

glm::vec4 v_positions_larm2[36];
glm::vec4 v_colors_larm2[36];

glm::vec4 v_positions_rarm1[36];
glm::vec4 v_colors_rarm1[36];

glm::vec4 v_positions_rarm2[36];
glm::vec4 v_colors_rarm2[36];

glm::vec4 v_positions_neck[36];
glm::vec4 v_colors_neck[36];

void init_cuboid(glm::vec4 *v_positions, glm::vec4 *v_colors, glm::vec4 centre, dimension dim, glm::vec4 color, glm::vec2* tex_coords = tex_coords_rider)
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
      {1, 0, 3, 2},
      {2, 3, 7, 6},
      {3, 0, 4, 7},
      {6, 5, 1, 2},
      {4, 5, 6, 7},
      {5, 4, 0, 1}};
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

float factor = 1;
float theight = 2.0 * factor; // torso's height
float swidth = 4.0 * factor;  // shoulder's width
float depth = 0.25 * factor;
float hwidth = 2.0 * factor;

glm::vec4 skin_color = glm::vec4(1.0, 0.86, 0.67, 1.0);
glm::vec4 shirt_color = glm::vec4(0.6, 0.4, 0.7, 1.0);
glm::vec4 shorts_color = glm::vec4(1.0, 0.0, 0.0, 1.0);

void init_torso()
{

  init_cuboid(v_positions_torso,
              v_colors_torso,
              glm::vec4(0.0, theight / 2.0, 0.0, 1.0),
              dimension(theight, theight / 2.0, depth),
              shirt_color);
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
//-----------------------------------------------------------------

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
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  texCoord = glGetAttribLocation( shaderProgram, "texCoord" ); 
  texFlag = glGetAttribLocation( shaderProgram, "tex_flag" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

  GLuint tex=LoadTexture("./src/images/track.bmp",50,50);
  glBindTexture(GL_TEXTURE_2D, tex);
  
  // Creating the hierarchy:
  // We are using the original colorcube function to generate the vertices of the cuboid
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

  hip = new csX75::HNode(NULL, num_vertices, v_positions_hip, v_colors_hip, pos_size, col_size);
  torso = new csX75::HNode(hip, num_vertices, v_positions_torso, v_colors_torso, pos_size, col_size,DOFs(),tex_coords_rider,sizeof(tex_coords_rider));

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

  root_node = hip;
  curr_node = hip;
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  // Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f, 0.0f, 0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f, 1.0f, 0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f, 0.0f, 1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * c_rotation_matrix;
  // Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos), glm::vec3(0.0), glm::vec3(c_up));

  // creating the projection matrix
  if (enable_perspective)
    projection_matrix = glm::frustum(-7.0, 7.0, -7.0, 7.0, 1.0, 7.0);
  // projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  else
    projection_matrix = glm::ortho(-7.0, 7.0, -7.0, 7.0, -5.0, 5.0);

  view_matrix = projection_matrix * lookat_matrix;

  matrixStack.push_back(view_matrix);

  hip->render_tree();
}

int main(int argc, char **argv)
{
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
