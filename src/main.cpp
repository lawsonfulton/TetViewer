#include <igl/opengl/glfw/Viewer.h>
#include <igl/readMSH.h>
#include <igl/readMESH.h>

#include "lf_utils.h"

#include <iostream>

using namespace Eigen;
	
// Input polygon
Eigen::MatrixXd V;
Eigen::MatrixXd B;
Eigen::MatrixXi F;
Eigen::MatrixXi T;

// This function is called every time a keyboard button is pressed
bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
	 using namespace std;
	using namespace Eigen;
	 if (key >= '1' && key <= '9')
	{
		double t = double((key - '1')+1) / 9.0;
		 VectorXd v = B.col(2).array() - B.col(2).minCoeff();
		v /= v.col(0).maxCoeff();
		 vector<int> s;
		 for (unsigned i=0; i<v.size();++i)
			if (v(i) < t)
				s.push_back(i);
		 MatrixXd V_temp(s.size()*4,3);
		MatrixXi F_temp(s.size()*4,3);
		 for (unsigned i=0; i<s.size();++i)
		{
			V_temp.row(i*4+0) = V.row(T(s[i],0));
			V_temp.row(i*4+1) = V.row(T(s[i],1));
			V_temp.row(i*4+2) = V.row(T(s[i],2));
			V_temp.row(i*4+3) = V.row(T(s[i],3));
			F_temp.row(i*4+0) << (i*4)+0, (i*4)+1, (i*4)+3;
			F_temp.row(i*4+1) << (i*4)+0, (i*4)+2, (i*4)+1;
			F_temp.row(i*4+2) << (i*4)+3, (i*4)+2, (i*4)+0;
			F_temp.row(i*4+3) << (i*4)+1, (i*4)+2, (i*4)+3;
		}
		 viewer.data().clear();
		viewer.data().set_mesh(V_temp,F_temp);
		viewer.data().set_face_based(true);
	}
	 return false;
}

int main(int argc, char *argv[])
{

	if(argc != 2) {
		std::cout << "Usage: ./TetViewer <filename>.msh/.mesh" << std::endl;
	}
	
	std::string meshPath = argv[1];
	if(lf::str::endsWith(meshPath, ".msh")) {
		igl::readMSH(meshPath, V, T);
	} else if (lf::str::endsWith(meshPath, ".mesh")) {
		igl::readMESH(meshPath, V, T, F);
	} else {
		std::cout << "Filetype must be .msh or .mesh" << std::endl;
	}

	// Compute barycenters
	igl::barycenter(V,T,B);

	// Plot the generated mesh
	igl::opengl::glfw::Viewer viewer;

	viewer.data().clear();
	viewer.data().set_mesh(V, F);
	viewer.data().set_face_based(true);

	viewer.callback_key_down = &key_down;
	key_down(viewer,'5',0);
	viewer.launch();
}