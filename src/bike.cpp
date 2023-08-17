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

#include "bike.hpp"

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
glm::vec2 t_coords[4] = {
  glm::vec2( 0.0, 0.0),
  glm::vec2( 0.0, 1.0),
  glm::vec2( 1.0, 0.0),
  glm::vec2( 1.0, 1.0)
};


//Constants
const float pi = M_PI;
const int v_num_wheel = 360 * 360 * 6;
const int v_num_spoke = 360 * 12;
const int v_num_fender = 24;
const int v_num_body = 36;
const int v_num_handlebar = v_num_spoke;
const int v_num_handles = v_num_handlebar;

// Parameters
float factor = 1;
float body_w = factor*4,body_h = factor*2,body_d = factor*0.4;
float engine_w = 2,engine_h = 2,engine_d = body_d;
float wheel_r = body_w/3,wheel_R = wheel_r + body_d;
float spoke_h = wheel_r,spoke_r = wheel_r/25;
float fender_h = body_h,fender_w = fender_h*1.4,fender_d = body_d;
float handlebars_h = 2.2*body_h,handlebars_r = 1.4*body_d/2;
float handles_h = 10*body_d,handles_r = handlebars_r;

//Colors
glm::vec4 color_wheel = glm::vec4(0,0,1,1);
glm::vec4 color_spoke = glm::vec4(1,0,0,1);
glm::vec4 color_fender = glm::vec4(0.9, 0, 0, 1);
glm::vec4 color_body = glm::vec4(1, 0, 0, 1);
glm::vec4 color_handlebars = glm::vec4(1,0,0,1);
glm::vec4 color_handles = glm::vec4(0,1,0,1);

//Vertices
glm::vec4 *cylinder_vertices;

glm::vec4 v_positions_wheel[v_num_wheel];
glm::vec4 v_colors_wheel[v_num_wheel];

glm::vec4 v_positions_spoke[v_num_spoke];
glm::vec4 v_colors_spoke[v_num_spoke];

glm::vec4 v_positions_fender[v_num_fender];
glm::vec4 v_colors_fender[v_num_fender];

glm::vec4 v_positions_body[v_num_body];
glm::vec4 v_colors_body[v_num_body];
glm::vec2 tex_coords_bike[v_num_body];


glm::vec4 v_positions_handlebars[v_num_handlebar];
glm::vec4 v_colors_handlebars[v_num_handlebar];

glm::vec4 v_positions_handles[v_num_handles];
glm::vec4 v_colors_handles[v_num_handles];


void init_fender(float h,float b,float w){
    glm::vec4 positions[6]={
        glm::vec4(-b,h,w,1),
        glm::vec4(0,h,w,1),
        glm::vec4(0,0,w,1),
        glm::vec4(-b,h,0,1),
        glm::vec4(0,h,0,1),
        glm::vec4(0,0,0,1)
    };
    for(int i=0;i<3;i++){
        v_positions_fender[i] = positions[i];
        v_positions_fender[i+3] = positions[i+3];
    }
    std::vector<std::vector<int>> v = {
        {0,3,5,2},
        {1,2,5,4},
        {0,1,4,3}
    };
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
    for(int i=0;i<v_num_fender;i++){
        v_colors_fender[i] = color_fender;
    }
}

void init_wheel(float r_, float R_)
{
    glm::vec4 torus_vertices[360 * 360];
    float r = (R_ - r_)/2, R = r_ + r;
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

void init_cylinder(float h,float r,glm::vec4 *v_positions,glm::vec4* v_colors,glm::vec4 color,int hor=0){
    cylinder_vertices = new glm::vec4[2 * 360];
    for(int i=0;i<360;i++){
        float u = i * pi / 180;
        float x = r * cos(u), z = r * sin(u);
        if(hor){
            cylinder_vertices[i] = glm::vec4(x,z,h/2,1.0);
            cylinder_vertices[i + 360] = glm::vec4(x,z,-h/2,1.0);            
            continue;
        }
        cylinder_vertices[i] = glm::vec4(x,h,z,1.0);
        cylinder_vertices[i + 360] = glm::vec4(x,0,z,1.0);
    }
    glm::vec4 top_centre = glm::vec4(0,h,0,1), bottom_centre = glm::vec4(0,0,0,1);
    if(hor){
        top_centre = glm::vec4(0,0,h/2,1);
        bottom_centre = glm::vec4(0,0,-h/2,1);
    }
    int tri_idx = 0;
    for(int i=0;i<360;i++){
        v_positions[tri_idx] = cylinder_vertices[i]; tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i+1)%360]; tri_idx++;
        v_positions[tri_idx] = top_centre; tri_idx++;

        v_positions[tri_idx] = cylinder_vertices[i+360]; tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i+1)%360 + 360]; tri_idx++;
        v_positions[tri_idx] = bottom_centre; tri_idx++;

        v_positions[tri_idx] = cylinder_vertices[i]; tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i+1)%360]; tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[i+360]; tri_idx++;

        v_positions[tri_idx] = cylinder_vertices[i+360]; tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i+1)%360 + 360]; tri_idx++;
        v_positions[tri_idx] = cylinder_vertices[(i+1)%360]; tri_idx++;

    }

    for(int i=0;i<v_num_spoke;i++){
        v_colors[i] = color;
    }

    delete cylinder_vertices;
    
}

void init_cuboid(glm::vec4 *v_positions, glm::vec4 *v_colors, glm::vec4 centre, dimension dim, glm::vec4 color, glm::vec2* tex_coords = tex_coords_bike)
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

void init_body(){
    init_cuboid(v_positions_body,
                v_colors_body,
                glm::vec4(0,0,0,1),
                dimension(body_h,body_w,body_d),
                color_body);
}

float theight = 2.0; // torso's height
float swidth = 4.0;  // shoulder's width
float depth = 0.25;
float hwidth = 2.0;

glm::vec4 skin_color = glm::vec4(1.0, 0.86, 0.67, 1.0);
glm::vec4 shirt_color = glm::vec4(0.6, 0.4, 0.7, 1.0);
glm::vec4 shorts_color = glm::vec4(1.0, 0.0, 0.0, 1.0);


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
    vNormal = glGetAttribLocation( shaderProgram, "vNormal" ); 
    normalMatrix =  glGetUniformLocation( shaderProgram, "normalMatrix");
    viewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix");
    uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

    GLuint tex=LoadTexture("./src/images/bike.bmp",256,256);
    glBindTexture(GL_TEXTURE_2D, tex);
  

    // Creating the hierarchy:
    // We are using the original colorcube function to generate the vertices of the cuboid

    init_wheel(wheel_r, wheel_R);
    init_fender(fender_h,fender_w,fender_d);
    init_body();
    // note that the buffers are initialized in the respective constructors
    body = new csX75::HNode(NULL, v_num_body, v_positions_body, v_colors_body, sizeof(v_positions_body), sizeof(v_colors_body),DOFs(),tex_coords_bike,sizeof(tex_coords_bike));
    //body->change_parameters(0,0,0,0,-40,0);
    fender = new csX75::HNode(body, v_num_fender, v_positions_fender, v_colors_fender, sizeof(v_positions_fender), sizeof(v_colors_fender));
    fender->change_parameters(-body_w/2,-body_h/2,-body_d/2,0,0,0);
    

    init_cylinder(handlebars_h,handlebars_r,v_positions_handlebars,v_colors_handlebars,color_handlebars);
    handlebar = new csX75::HNode(body, v_num_wheel, v_positions_handlebars, v_colors_handlebars, sizeof(v_positions_handlebars), sizeof(v_colors_handlebars),DOFs(0,0,-360,360,0,0,1));
    handlebar->change_parameters(body_w/2+fender_w/5,-0.8*body_h,0,0,0,16);

    init_cylinder(handles_h,handles_r,v_positions_handles,v_colors_handles,color_handles,1);
    handles = new csX75::HNode(handlebar, v_num_wheel, v_positions_handles, v_colors_handles, sizeof(v_positions_handles), sizeof(v_colors_handles));
    handles->change_parameters(-0.7*handlebars_r,handlebars_h*0.9,0,0,0,0);

    init_cylinder(spoke_h,spoke_r,v_positions_spoke,v_colors_spoke,color_spoke);

    wheels[0] = new csX75::HNode(body, v_num_wheel, v_positions_wheel, v_colors_wheel, sizeof(v_positions_wheel), sizeof(v_colors_wheel),DOFs(0,0,0,0,-360,360));
    wheels[0]->change_parameters(-body_w/2-fender_w/2,-0.8*body_h,0,0,0,0);
    
    wheels[1] = new csX75::HNode(handlebar, v_num_wheel, v_positions_wheel, v_colors_wheel, sizeof(v_positions_wheel), sizeof(v_colors_wheel),DOFs(0,0,0,0,-360,360));
    wheels[1]->change_parameters(0,0,0,0,0,0);
    for(int j=0;j<2;j++){
        for(int i=0;i<num_spokes;i++){
            spokes[i+j*num_spokes] = new csX75::HNode(wheels[j], v_num_spoke, v_positions_spoke, v_colors_spoke, sizeof(v_positions_spoke), sizeof(v_colors_spoke));
            float u = i * 360/(num_spokes);
            spokes[i+j*num_spokes]->change_parameters(0,0,0,0,0,u);
        }
    }

    root_node = body;
    curr_node = body;
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
        projection_matrix = glm::ortho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

    view_matrix = projection_matrix * lookat_matrix;

    glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
    normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
    glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
    matrixStack.push_back(view_matrix);

    root_node->render_tree();
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
