#ifndef MARCHINGCUBE_H
#define MARCHINGCUBE_H
#include <vector>

void MarchingCube(std::vector<float> &input,
		  std::vector<float> &output_vertices,
		  std::vector<float> &output_normals);

#endif