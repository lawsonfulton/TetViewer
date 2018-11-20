#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>

#include "lf_utils.h"

// Input polygon
Eigen::MatrixXd V;
Eigen::MatrixXi F;
Eigen::MatrixXi T;

// This function is called every time a keyboard button is pressed
bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
  return false;
}

int main(int argc, char *argv[])
{
  using namespace Eigen;
  using namespace std;

  // Load a surface mesh
  igl::read_triangle_mesh(argv[1], V, F);

  // Plot the generated mesh
  igl::opengl::glfw::Viewer viewer;

  viewer.data().clear();
  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);

  viewer.callback_key_down = &key_down;
  key_down(viewer,'5',0);
  viewer.launch();
}