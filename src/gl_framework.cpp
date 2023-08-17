#include "gl_framework.hpp"
#include "hierarchy_node.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

extern Camera *curr_cam, *cam1, *cam2, *cam3;
extern bool enable_perspective;
extern csX75::HNode *straight1, *torso, *hip, *shoulder, *lleg1, *lleg2, *rleg1, *rleg2, *larm1, *larm2, *rarm1, *rarm2, *neck, *head, *node2, *node3, *curr_node;
extern csX75::HNode *wheels[2], **spokes, *fender, *engine, *body, *handlebar, *handles;
extern void renderGL();
extern int mode;
int timestamp = 0;
int cam_idx = 0;
int rotspeed = 5;
namespace csX75
{

  struct KeyFrame
  {
    int timestamp, mode, cam_idx;
    GLfloat cxpos, cypos, czpos;
    GLfloat hxpos, hypos, hzpos;
    GLfloat bxpos, bypos, bzpos;

    GLfloat cxrot, cyrot, czrot;
    GLfloat hxrot, hyrot, hzrot;
    GLfloat bxrot, byrot, bzrot;

    glm::vec3 clook_at;
    KeyFrame(int timestamp1, int mode1, int cam_idx1, GLfloat cxpos1, GLfloat cypos1, GLfloat czpos1, GLfloat hxpos1, GLfloat hypos1, GLfloat hzpos1, GLfloat bxpos1, GLfloat bypos1, GLfloat bzpos1,
             GLfloat cxrot1, GLfloat cyrot1, GLfloat czrot1, GLfloat hxrot1, GLfloat hyrot1, GLfloat hzrot1, GLfloat bxrot1, GLfloat byrot1, GLfloat bzrot1,
             glm::vec3 clook_at1)
    {
      cam_idx = cam_idx1;
      cxpos = cxpos1;
      cypos = cypos1;
      czpos = czpos1;

      hxpos = hxpos1;
      hypos = hypos1;
      hzpos = hzpos1;

      bxpos = bxpos1;
      bypos = bypos1;
      bzpos = bzpos1;

      cxrot = cxrot1;
      cyrot = cyrot1;
      czrot = czrot1;

      hxrot = hxrot1;
      hyrot = hyrot1;
      hzrot = hzrot1;

      bxrot = bxrot1;
      byrot = byrot1;
      bzrot = bzrot1;

      clook_at = clook_at1;
      timestamp = timestamp1;
      mode = mode1;
    }
  };
  std::vector<KeyFrame> kf;
  std::vector<KeyFrame> kf_interpolated;

  //! Initialize GL  State
  void
  initGL(void)
  {
    // Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Set depth buffer furthest depth
    glClearDepth(1.0);
    // Set depth test to less-than
    glDepthFunc(GL_LESS);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }

  //! GLFW Error Callback
  void error_callback(int error, const char *description)
  {
    std::cerr << description << std::endl;
  }

  //! GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow *window, int width, int height)
  {
    //! Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }

  void saveKeyframe()
  {

    std::ofstream myfile;
    myfile.open("./keyframes.txt", std::ios::app);
    std::cout << "saving " << hip->tx << "\n";
    if (curr_cam == cam1)
    {
      cam_idx = 1;
    }
    else if (curr_cam == cam2)
    {
      cam_idx = 2;
    }
    else if (curr_cam == cam3)
    {
      cam_idx = 3;
    }

    myfile << timestamp << " " << mode << " " << cam_idx << " "
           << curr_cam->xpos << " " << curr_cam->ypos << " " << curr_cam->zpos << " "
           << curr_cam->xrot << " " << curr_cam->yrot << " " << curr_cam->zrot << " "
           << curr_cam->look_at[0] << " " << curr_cam->look_at[1] << " " << curr_cam->look_at[2] << " "
           << body->tx << " " << body->ty << " " << body->tz << " "
           << body->rx << " " << body->ry << " " << body->rz << " "
           << hip->tx << " " << hip->ty << " " << hip->tz << " "
           << hip->rx << " " << hip->ry << " " << hip->rz << "\n";

    myfile.close();
    timestamp += 15;

    // timestamp
    // curr cam
    // cam vars for each - x,y,z, xr, yr, zr
    // rider variables - hip
    // bike variables - body
  }

  void renderFrame(KeyFrame frame)
  {
    mode = frame.mode;
    cam_idx = frame.cam_idx;
    if (cam_idx == 1)
    {
      curr_cam = cam1;
    }
    else if (cam_idx == 2)
    {
      curr_cam = cam2;
    }
    else
    {
      curr_cam = cam3;
    }
    curr_cam->xpos = frame.cxpos;
    curr_cam->ypos = frame.cypos;
    curr_cam->zpos = frame.czpos;

    curr_cam->xrot = frame.cxrot;
    curr_cam->yrot = frame.cyrot;
    curr_cam->zrot = frame.czrot;

    curr_cam->look_at = frame.clook_at;

    body->tx = frame.bxpos;
    body->ty = frame.bypos;
    body->tz = frame.bzpos;

    body->rx = frame.bxrot;
    body->ry = frame.byrot;
    body->rz = frame.bzrot;

    hip->tx = frame.hxpos;
    hip->ty = frame.hypos;
    hip->tz = frame.hzpos;

    hip->rx = frame.hxrot;
    hip->ry = frame.hyrot;
    hip->rz = frame.hzrot;

    hip->update_matrices();
    body->update_matrices();
  }

  void saveImage(char *filepath, GLFWwindow *w)
  {
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    GLsizei nrChannels = 4;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
  }

  std::vector<KeyFrame> loadKeyframes()
  {
    std::vector<KeyFrame> keyframes;
    std::ifstream infile("keyframes.txt");
    int linecount = 0;

    GLfloat cxpos, cypos, czpos;
    GLfloat hxpos, hypos, hzpos;
    GLfloat bxpos, bypos, bzpos;

    GLfloat cxrot, cyrot, czrot;
    GLfloat hxrot, hyrot, hzrot;
    GLfloat bxrot, byrot, bzrot;

    glm::vec3 clook_at;

    while (infile >> timestamp >> mode >> cam_idx >>
           cxpos >> cypos >> czpos >>
           cxrot >> cyrot >> czrot >>
           clook_at[0] >> clook_at[1] >> clook_at[2] >>
           bxpos >> bypos >> bzpos >>
           bxrot >> byrot >> bzrot >>
           hxpos >> hypos >> hzpos >>
           hxrot >> hyrot >> hzrot)
    {
      linecount++;

      KeyFrame k = KeyFrame(timestamp,
                            mode,
                            cam_idx,
                            cxpos,
                            cypos,
                            czpos,

                            hxpos,
                            hypos,
                            hzpos,

                            bxpos,
                            bypos,
                            bzpos,

                            cxrot,
                            cyrot,
                            czrot,

                            hxrot,
                            hyrot,
                            hzrot,

                            bxrot,
                            byrot,
                            bzrot,

                            clook_at

      );

      keyframes.push_back(k);
    }
    std::cout << linecount << " frames loaded!\n";
    return keyframes;
  }
  /*
  Controls:

  t: torso
  n: neck
  h: hip
  0: shoulder
  1: head
  2: lleg1
  3: lleg2
  4: rleg1
  5: rleg2
  6: larm1
  7: larm2
  8: rarm1
  9: rarm2

  */

  //! GLFW keyboard callback
  void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
      std::cout << "Saving frame..."
                << "\n";
      char filepath[50] = {"./frames/frame.png"};
      saveImage(filepath, window);
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
      saveKeyframe();
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
      kf = loadKeyframes();
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
      for (int i = 0; i < kf.size(); i++)
      {
        kf_interpolated.push_back(kf[i]);
        KeyFrame k1 = kf[i];
        KeyFrame k2 = kf[i + 1];
        float f = 1/15.0;
        float curr_f = f;
        for(int j=1; j<=14; j++){
          kf_interpolated.push_back(KeyFrame(

            k1.timestamp+j,
            2,
            k1.cam_idx,
            k1.cxpos,
            k1.cypos,
            k1.czpos,

            curr_f* k1.hxpos  + (1-curr_f) * k2.hxpos,
            curr_f*k1.hypos  + (1-curr_f) * k2.hypos,
            curr_f*k1.hzpos  + (1-curr_f) * k2.hzpos,


            curr_f*k1.bxpos  + (1-curr_f) * k2.bxpos,
            curr_f*k1.bypos  +(1-curr_f) *  k2.bypos,
            curr_f*k1.bzpos  +(1-curr_f) *  k2.bzpos,

            k1.cxrot,
            k1.cyrot,
            k1.czrot,

            curr_f*k1.hxrot  + (1-curr_f) * k2.hxrot,
            curr_f*k1.hyrot  + (1-curr_f) * k2.hyrot,
            curr_f*k1.hzrot  + (1-curr_f) * k2.hzrot,

            curr_f*k1.bxrot  + (1-curr_f) * k2.bxrot,
            curr_f*k1.byrot  +(1-curr_f) *  k2.byrot,
            curr_f*k1.bzrot  + (1-curr_f) * k2.bzrot,

            k1.clook_at
          ));
        }

      }
      // std::cout << kf.size() << "\n";
      int cnt{1};
      for (auto frame : kf_interpolated)
      {
        std::cout<<cnt<<"\n";
        //glfwWaitEventsTimeout(0.01);
        renderFrame(frame);
        renderGL();
        glfwSwapBuffers(window);
        std::string fname = "./images/frame_"+std::to_string(cnt)+".png";
        char fname_[fname.length()];
        strcpy(fname_,fname.c_str());
        saveImage(fname_,window);
        cnt++;
      }

      return;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
      curr_node = hip;
      mode = 0;
    }

    else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
      curr_node = straight1;
      mode = 2;
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
      curr_node = body;
      mode = 1;
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS && mods == GLFW_MOD_SHIFT)
    {
      curr_cam = cam1;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS && mods == GLFW_MOD_SHIFT)
    {
      curr_cam = cam2;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS && mods == GLFW_MOD_SHIFT)
    {
      curr_cam = cam3;
    }
    if (mode == 0) // rider
    {
      //! Close the window if the ESC key was pressed
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
      else if (key == GLFW_KEY_T && action == GLFW_PRESS)
        curr_node = torso;
      else if (key == GLFW_KEY_N && action == GLFW_PRESS)
        curr_node = neck;
      else if (key == GLFW_KEY_H && action == GLFW_PRESS)
        curr_node = hip;
      else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        curr_node = shoulder;
      else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        curr_node = head;
      else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        curr_node = lleg1;
      else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        curr_node = lleg2;
      else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        curr_node = rleg1;
      else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        curr_node = rleg2;
      else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        curr_node = larm1;
      else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
        curr_node = larm2;
      else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
        curr_node = rarm1;
      else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
        curr_node = rarm2;
      else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        curr_node->dec_ry();
      else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        curr_node->inc_ry();
      else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        curr_node->dec_rx();
      else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        curr_node->inc_rx();
      else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
        curr_node->dec_rz();
      else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
        curr_node->inc_rz();
      else if (key == GLFW_KEY_A && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          hip->dec_tx();
          return;
        }
        curr_cam->yrot -= rotspeed;
      }
      else if (key == GLFW_KEY_D && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          hip->inc_tx();
          return;
        }
        curr_cam->yrot += rotspeed;
      }
      else if (key == GLFW_KEY_W && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          hip->inc_ty();
          return;
        }
        curr_cam->xrot -= rotspeed;
      }
      else if (key == GLFW_KEY_X && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          hip->dec_ty();
          return;
        }
        curr_cam->xrot += rotspeed;
      }
      else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          hip->dec_tz();
          return;
        }
        curr_cam->zrot -= rotspeed;
      }
      else if (key == GLFW_KEY_E && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          hip->inc_tz();
          return;
        }
        curr_cam->zrot += rotspeed;
      }
    }
    else if (mode == 1) // bike
    {
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
      else if (key == GLFW_KEY_B && action == GLFW_PRESS)
        curr_node = body;
      else if (key == GLFW_KEY_H && action == GLFW_PRESS)
        curr_node = handlebar;
      else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        curr_node = wheels[0];
      else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        curr_node = wheels[1];
      else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        curr_node->dec_ry();
      else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        curr_node->inc_ry();
      else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        curr_node->dec_rx();
      else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        curr_node->inc_rx();
      else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
        curr_node->dec_rz();
      else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
        curr_node->inc_rz();
      else if (key == GLFW_KEY_A && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          body->dec_tx();
          return;
        }
        curr_cam->yrot -= rotspeed;
      }
      else if (key == GLFW_KEY_D && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          body->inc_tx();
          return;
        }
        curr_cam->yrot += rotspeed;
      }
      else if (key == GLFW_KEY_W && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          body->inc_ty();
          return;
        }
        curr_cam->xrot -= rotspeed;
      }
      else if (key == GLFW_KEY_X && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          body->dec_ty();
          return;
        }
        curr_cam->xrot += rotspeed;
      }
      else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          body->dec_tz();
          return;
        }
        curr_cam->zrot -= rotspeed;
      }
      else if (key == GLFW_KEY_E && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          body->inc_tz();
          return;
        }
        curr_cam->zrot += rotspeed;
      }
    }
    else if (mode == 2) // track
    {
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
      else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
      {
        curr_cam = cam1;
      }
      else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
      {
        curr_cam = cam2;
      }
      else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
      {
        curr_cam = cam3;
      }
      else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        curr_node->dec_ry();
      else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        curr_node->inc_ry();
      else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        curr_node->dec_rx();
      else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        curr_node->inc_rx();
      else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
        curr_node->dec_rz();
      else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
        curr_node->inc_rz();
      else if (key == GLFW_KEY_A && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          curr_node->dec_tx();
          return;
        }
        curr_cam->yrot -= rotspeed;
      }
      else if (key == GLFW_KEY_D && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          curr_node->inc_tx();
          return;
        }
        curr_cam->yrot += rotspeed;
      }
      else if (key == GLFW_KEY_W && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          curr_node->inc_ty();
          return;
        }
        curr_cam->xrot -= rotspeed;
      }
      else if (key == GLFW_KEY_X && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          curr_node->dec_ty();
          return;
        }
        curr_cam->xrot += rotspeed;
      }
      else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          curr_node->dec_tz();
          return;
        }
        curr_cam->zrot -= rotspeed;
      }
      else if (key == GLFW_KEY_E && action == GLFW_PRESS)
      {
        if (mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_CAPS_LOCK)
        {
          curr_node->inc_tz();
          return;
        }
        curr_cam->zrot += rotspeed;
      }
    }
  }
};
