#include "MarchingCube.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;


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
		  std::vector<float> &output_normals, char threshold){

  // fill the outputs with results applying Marching cube on input

  /*

A diagram for convenience while planning.

z
^    _  y
|    /|
|  /
|/______> X
     ________
   /|       /|
 /__|____ /  |
|   |	 |   |
|   |____|___|
|  /     |  /    
|/_______|/

  */

  //Work in progress: real marching cubes.

  for(int currentX = 0; currentX < dim[0] - 1; currentX++)
	for(int currentY = 0; currentY < dim[1] - 1; currentY++)
		for (int currentZ = 0; currentZ < dim[2] - 1; currentZ++)
		{
			/*
				Some assumptions I am making about the array shape.
				--------> x direction
				|
				|
				|
				V y direction
			*/
			char cornerLowerLeftFront = input[currentX + currentY * dim[0] + currentZ * dim[0] * dim[1]];
			char cornerLowerRightFront = input[currentX + 1 + currentY * dim[0] + currentZ * dim[0] * dim[1]];
			char cornerLowerLeftBack = input[currentX + (currentY + 1) * dim[0] + currentZ * dim[0] * dim[1]];
			char cornerLowerRightBack = input[currentX + 1 + (currentY + 1) * dim[0] + currentZ * dim[0] * dim[1]];
			char cornerUpperLeftFront = input[currentX + currentY * dim[0] + (currentZ + 1) * dim[0] * dim[1]];
			char cornerUpperRightFront = input[currentX + 1 + currentY * dim[0] + (currentZ + 1) * dim[0] * dim[1]];
			char cornerUpperLeftBack = input[currentX + (currentY + 1) * dim[0] + (currentZ + 1) * dim[0] * dim[1]];
			char cornerUpperRightBack = input[currentX + 1 + (currentY + 1) * dim[0] + (currentZ + 1) * dim[0] * dim[1]];
			bool cornerLowerLeftFrontAboveThreshold = cornerLowerLeftFront > threshold;
			bool cornerLowerRightFrontAboveThreshold = cornerLowerRightFront > threshold;
			bool cornerLowerLeftBackAboveThreshold = cornerLowerLeftBack > threshold;
			bool cornerLowerRightBacktAboveThreshold = cornerLowerRightBack > threshold;
			bool cornerUpperLeftFrontAboveThreshold = cornerUpperLeftFront > threshold;
			bool cornerUpperRightFrontAboveThreshold = cornerUpperRightFront > threshold;
			bool cornerUpperLeftBackAboveThreshold = cornerUpperLeftBack > threshold;
			bool cornerUpperRightBackAboveThreshold = cornerUpperRightBack > threshold;	
			int howManyCornersAbove = 0;
			if(cornerLowerLeftFrontAboveThreshold)
				howManyCornersAbove++;
			if(cornerLowerRightFrontAboveThreshold) 
				howManyCornersAbove++;
			if(cornerLowerLeftBackAboveThreshold) 
				howManyCornersAbove++;
			if(cornerLowerRightBacktAboveThreshold)
				howManyCornersAbove++;
			if(cornerUpperLeftFrontAboveThreshold) 
				howManyCornersAbove++;
			if(cornerUpperRightFrontAboveThreshold) 
				howManyCornersAbove++;
			if(cornerUpperLeftBackAboveThreshold) 
				howManyCornersAbove++;
			if(cornerUpperRightBackAboveThreshold) 
				howManyCornersAbove++;
			if (howManyCornersAbove != 0 && howManyCornersAbove != 8)
			{
				output_vertices.push_back(float(currentX)/float(dim[0]));
				output_vertices.push_back(float(currentY) / float(dim[0]));
				output_vertices.push_back(float(currentZ) / float(dim[0]));

				output_vertices.push_back(float(currentX + 1) / float(dim[0]));
				output_vertices.push_back(float(currentY) / float(dim[0]));
				output_vertices.push_back(float(currentZ) / float(dim[0]));

				output_vertices.push_back(float(currentX) / float(dim[0]));
				output_vertices.push_back(float(currentY) / float(dim[0]));
				output_vertices.push_back(float(currentZ + 1) / float(dim[0]));
			}
			//cout << std::to_string(howManyCornersAbove) + "\n";
		}	

  // this is a debug thing to show the frog contour
  //simpleSurface(input, dim, output_vertices, output_normals);

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
