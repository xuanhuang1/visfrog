#include "MarchingCube.h"
#include <vector>


void MarchingCube(std::vector<float> &input,
		  std::vector<float> &output_vertices,
		  std::vector<float> &output_normals){
  // fill the outputs with results applying MC on input
  // this line just fill in one hard coded triangle
  output_vertices.push_back(0.0);
  output_vertices.push_back(0.5);
  output_vertices.push_back(0.0);
  
  output_vertices.push_back(0.5);
  output_vertices.push_back(-0.5);
  output_vertices.push_back(0.0);

  output_vertices.push_back(-0.5);
  output_vertices.push_back(-0.5);
  output_vertices.push_back(0.0);
}
