#include "MarchingCube.h"
#include <vector>


// form a front-facing triangle per non zero vertex
// should give a vague frog shape
void simpleSurface(const std::vector<char> &input,
      const int dim[3], // x y z dimension of the data
      std::vector<float> &output_vertices,
      std::vector<float> &output_normals);



// the marching cube algorithm
// 
void MarchingCube(const std::vector<char> &input,
      const int dim[3], // x y z dimension of the data
		  std::vector<float> &output_vertices,
		  std::vector<float> &output_normals){

  // fill the outputs with results applying Marching cube on input

  // this is a debug thing to show the frog contour
  simpleSurface(input, dim, output_vertices, output_normals);

}











// form a front-facing triangle per non zero vertex
// should give a vague frog shape

void simpleSurface(const std::vector<char> &input,
      const int dim[3], // x y z dimension of the data
      std::vector<float> &output_vertices,
      std::vector<float> &output_normals){

  for (int i = 0; i < dim[0] * dim[1] * dim[2]; ++i)
  {

    if (input[i]){
    int position[3];
    position[0] = i % dim[0];
    position[1] = i / dim[0] % dim[1];
    position[2] = i / (dim[0] * dim[1]);

    float position_unit[3];
    for (int j = 0; j < 3; ++j) 
      position_unit[j] = float(position[j]) / dim[0]- 0.5f; // x is the largest

    float triangle_size = 0.01;

    output_vertices.push_back(position_unit[0]);
    output_vertices.push_back(position_unit[1] + triangle_size);
    output_vertices.push_back(position_unit[2]);
  
    output_vertices.push_back(position_unit[0] + triangle_size);
    output_vertices.push_back(position_unit[1] - triangle_size);
    output_vertices.push_back(position_unit[2]);


    output_vertices.push_back(position_unit[0] - triangle_size);
    output_vertices.push_back(position_unit[1] - triangle_size);
    output_vertices.push_back(position_unit[2]);
    }
  }

}
