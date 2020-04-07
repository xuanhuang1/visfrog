// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>

#include <string.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

// include MC 
#include "isosurface/MarchingCube.h"

const char* vertex_shader =
"#version 330\n"
"uniform mat4 MVP;"
"uniform mat4 V;"
"in vec3 vp;"
"void main() {"
"  gl_Position = MVP * vec4(vp, 1);"
"}";

const char* fragment_shader =
"#version 330\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.0, 0.4);"
"}";



// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static  float box_points[] = {
    -0.5f,-0.5f,-0.5f, // triangle 1 : begin
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
    0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
};

enum RENDER_TYPE{
	SURFACE, VOLUME
};

float width = 800, height = 600;
glm::mat4 view, MVP, model, projection;

RENDER_TYPE render_mode = SURFACE; // CHANGE HERE FOR A DIFFERENT MODE


GLuint getShader(const char* v, const char* f){
	// load shader program
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &v, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &f, NULL);
	glCompileShader(fs);

	GLuint shader_program_surface = glCreateProgram();
	glAttachShader(shader_program_surface, fs);
	glAttachShader(shader_program_surface, vs);
	glLinkProgram(shader_program_surface);
	return shader_program_surface;
}



GLuint getShaderFromFile(const char* path_v, const char* path_f){
	// load shader program
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	
	std::string content;
	std::ifstream fileStream(path_v, std::ios::in);

	if(!fileStream.is_open()) {
	  std::cerr << "Could not read file "
		    << path_v << ". File does not exist."
		    << std::endl;
	}

	std::string line = "";
	while(!fileStream.eof()) {
	  std::getline(fileStream, line);
	  content.append(line+"\n");
	}
	fileStream.close();
	
	const GLchar* v_code = content.c_str();
	
	glShaderSource(vs, 1, &v_code, NULL);
	glCompileShader(vs);


	std::string contentF = "";
	std::ifstream fileStreamF(path_f, std::ios::in);
	if(!fileStreamF.is_open()) {
	  std::cerr << "Could not read file "
		    << path_f << ". File does not exist."
		    << std::endl;
	}

	std::string lineF = "";
	while(!fileStreamF.eof()) {
	  std::getline(fileStreamF, lineF);
	  contentF.append(lineF+"\n");
	}
	fileStreamF.close();
	
	const GLchar* f_code = contentF.c_str();
	
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &f_code, NULL);
	glCompileShader(fs);

	GLuint shader_program_surface = glCreateProgram();
	glAttachShader(shader_program_surface, fs);
	glAttachShader(shader_program_surface, vs);
	glLinkProgram(shader_program_surface);
	return shader_program_surface;
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        render_mode = SURFACE;
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        render_mode = VOLUME;
}




int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow( width, height, "Tutorial 01", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Set Render Mode Callback
	glfwSetKeyCallback(window, key_callback);

	// Create and link shader 
	GLuint shader_program_surface = getShader(vertex_shader, fragment_shader);
	GLuint shader_program_rayTrace = getShaderFromFile("../../src/rayTrace/rayt.vert",
							   "../../src/rayTrace/rayt.frag");

	std::vector<float> vertices_surface;
	std::vector<float> normals_surface;

	int dim[3] = {500, 470, 136};
	std::vector<char> inputData(dim[0]*dim[1]*dim[2]);

	// Read data
	//std::ifstream file("../data/frog.raw", std::ios::in | std::ios::binary);
	std::ifstream file("../../data/frog.raw", std::ios::in | std::ios::binary);
	for (int i=0; i<dim[0]*dim[1]*dim[2]; i++){
    	file.read((&inputData[i]), sizeof(inputData[i]));
  	}
  	file.close();

  	// output test for correctness, should be exactly the same
  	// std::ofstream ofile ("output.raw", std::ios::out | std::ios::binary);
  	// if (ofile.is_open()){
   	//  ofile.write(&inputData[0], sizeof(inputData[0]) * dim[0] * dim [1] * dim[2]);
  	//  ofile.close();
  	// }

	// for test case make a simple 2x2x2 grid, half 0s half 1s
	MarchingCube(inputData, dim, vertices_surface, normals_surface, 150);



	// set texture
	glEnable(GL_TEXTURE_3D);
	PFNGLTEXIMAGE3DPROC glTexImage3D;
	glTexImage3D = (PFNGLTEXIMAGE3DPROC) glfwGetProcAddress("glTexImage3D");
	
	if(!GL_ARB_texture_non_power_of_two)
	  printf("NO NPOT TEXTURE EXTENSION!\n");
	  else printf("support not power of 2 extension\n");
	
	unsigned int texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_3D, texname);

	//GLubyte *texels = new GLubyte[inputData.size()];
	/*for (int i=0;i<6*6*3;i+=3){
	  box_points[i+1] *= ((dim[1]+0.0f)/dim[0]);
	  box_points[i+2] *= ((dim[2]+0.0f)/dim[0]);
	  }*/


	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, dim[0], dim[1], dim[2], 0, GL_RED, 
	  GL_UNSIGNED_BYTE, &inputData[0]);

	// Create vbos
	GLuint vbo_surface = 0;
	glGenBuffers(1, &vbo_surface);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_surface);
	glBufferData(GL_ARRAY_BUFFER, vertices_surface.size() * sizeof(float), &vertices_surface[0], GL_STATIC_DRAW);

	GLuint vbo_volume = 0;
	glGenBuffers(1, &vbo_volume);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_volume);
	glBufferData(GL_ARRAY_BUFFER, 3*6*6 * sizeof(float), box_points, GL_STATIC_DRAW);

	// Create vaos
	GLuint vao_surface = 0;
	glGenVertexArrays(1, &vao_surface);
	glBindVertexArray(vao_surface);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_surface);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint vao_volume = 0;
	glGenVertexArrays(1, &vao_volume);
	glBindVertexArray(vao_volume);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_volume);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	
	
	// init cameras
	glm::vec3 cameraPos = glm::vec3(1.0f, 1.0f, -2.0f);  
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 

	glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	
	view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
	projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);

	
	MVP =  projection * view; // just let model = I 


	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	GLuint shader_program = shader_program_rayTrace;
	GLuint vao = vao_surface;
	int drawArraySize = vertices_surface.size()/3;

	do{
		// Clear the screen
	  
	  glEnable(GL_DEPTH_TEST);
	  glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );

		if (render_mode == SURFACE){
			shader_program = shader_program_surface;
			vao = vao_surface;
			drawArraySize = vertices_surface.size()/3;
			  
		}else if (render_mode == VOLUME){
			shader_program = shader_program_rayTrace;
			vao = vao_volume;
			drawArraySize = 6*6;
			
			glUniform1i(glGetUniformLocation(shader_program, "vol"), 0); // set it manually
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_3D, texname);
		}
		// set shader to use
		glUseProgram(shader_program);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		
		// Get a handle for our "MVP" uniform
		GLuint MatrixID = glGetUniformLocation(shader_program, "MVP");
		GLuint ModelViewMatrixID = glGetUniformLocation(shader_program, "MV");
		GLuint EyePos = glGetUniformLocation(shader_program, "EyePos");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelViewMatrixID, 1, GL_FALSE, &view[0][0]);
		glUniform3fv(EyePos, 1, glm::value_ptr(cameraPos));

		
		glBindVertexArray(vao);
	  	glDrawArrays(GL_TRIANGLES, 0, drawArraySize);

		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vbo_surface);
	glDeleteBuffers(1, &vbo_volume);
	glDeleteProgram(shader_program_surface);
	glDeleteVertexArrays(1, &vao_surface);
	glDeleteVertexArrays(1, &vao_volume);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
