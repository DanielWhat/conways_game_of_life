#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
//build commamnd: g++ -Wall -o "%e" "%f" -lGL -lGLU -lglut -lGLEW

typedef struct game_state_s {
	GLuint vao_triangle;
	GLuint vbo_triangle[2];
	GLfloat colours[12] = {1.0, 0.0, 0.0, 1.0,
						 0.0, 1.0, 0.0, 1.0,
						 0.0, 0.0, 1.0, 1.0};
} GameState;

GLfloat verts[] =  //if you defined this in the initialise function you would need to use static const verts[]
{
	-0.8, -0.8,
	 0.8, -0.8,
	 0.0, 0.8
};

GameState game_state = GameState(); 

GLuint load_shader(GLenum shaderType, std::string filename)
{
	std::ifstream shader_file = std::ifstream(filename.c_str());
	if (!shader_file.good()) {
		std::cout << "Error opening shader file." << std::endl;
	}
	std::stringstream shader_data;
	shader_data << shader_file.rdbuf();
	shader_file.close();
	std::string shader_str = shader_data.str();
	const char* shader_text = shader_str.c_str();
	
	GLuint shader = glCreateShader(shaderType); //create a new shader
	glShaderSource(shader, 1, &shader_text, NULL); //basically just writes the code in our file to the shader
	glCompileShader(shader);
	
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status); //compile the shader
	if (status == GL_FALSE)
	{
		std::cout << "Problem compiling shader with filename: " << filename << std::endl;
		GLint info_log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		GLchar *str_info_log = new GLchar[info_log_length + 1];
		glGetShaderInfoLog(shader, info_log_length, NULL, str_info_log);
		std::cerr <<  "Compile failure in shader: " << str_info_log << std::endl;
		delete[] str_info_log;  //when deleting an array you should always use delete[] not just delete. delete[] tells the compiler it's not just a pointer to a single char
	}
	return shader;
}


void load_shaders()
{
	GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, "demo.vert");
	GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, "demo.frag");
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program); //link the shaders

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	
	if (status == GL_FALSE) {
		std::cout << "Problem linking shaders" << std::endl;
	}
	
	glUseProgram(program);
}
	


void display() 
{
	glClear(GL_COLOR_BUFFER_BIT); //clear the colour buffer
	
	glBindVertexArray(game_state.vao_triangle); //bind our triangle VAO (analogous to binding our triangle model since VAOs define everything about a model)
	glDrawArrays(GL_TRIANGLES, 0, 3); //draw triangles, start at index 0 and draw 3 vertices
	
	glutSwapBuffers();
}


void initialise() 
{	
	load_shaders();
	
	//VAOs (vertex array objects) are structures that contain all the information about an object/model
	//They are arrays that contain 1 or more VBO (vertex buffer objects).
	//VBOs are basically just pointers to buffer data (i.e vertex coordinates, vertex colours, vertex normals etc.)
	glGenVertexArrays(1, &game_state.vao_triangle); //generate a VAO
    glBindVertexArray(game_state.vao_triangle); //bind it to the current state (remember OpenGL works like a state machine)
    
    glGenBuffers(2, game_state.vbo_triangle); //generate 2 VBOs
    glBindBuffer(GL_ARRAY_BUFFER, game_state.vbo_triangle[0]); //bind it
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); //set up the buffer data, we use GL_STATIC_DRAW because we are not going to edit the vertex array
	
	glEnableVertexAttribArray(0); //enables the first vbo as a vertex attribute and tells openGL to use it when we call glDrawArrays
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL); //Just tells openGL a bit about the array/attribute we just enabled
	//says that for the first vbo (0th), every 2 elements in the vertex buffer define the position property, each element is a float, 
	//GL_FLASE for normalise (i.e don't normalise the values), 0 byte offset, and NULL=start at the start reading from the beggining of the array
	
	
	//I do the same as above now, but for the colour attribute
	glBindBuffer(GL_ARRAY_BUFFER, game_state.vbo_triangle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(game_state.colours), game_state.colours, GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set background colour to black
	
	//we have a 2d space, you only need to think about projections when you have a 3d scene and a 2d viewing port (i.e laptop screen)
	/*
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1, 1.0, 1000.0); //this is a matrix that correctly projects the 3d view space to 2d space for viewing on a screen 
    */
}



void kek(unsigned char key, int x, int y)
{
	if (key == 'a') {
		game_state.colours[1] = 1.0;
		glBindVertexArray(game_state.vao_triangle);
		glBindBuffer(GL_ARRAY_BUFFER, game_state.vbo_triangle[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(game_state.colours), game_state.colours);
		std::cout << "memes" << std::endl;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //initialise the window with RGB colours, a double buffer for smoother animations
    glutInitWindowSize(600, 600);
    glutCreateWindow("Conway's Game of Life");
    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glewInit(); //start glew
    
    glutKeyboardFunc(kek);
    initialise();
    glutDisplayFunc(display);
    glutMainLoop(); 
}


/* To do:
 * Add a grid and make each square a point sprite.
 * Instead of doing these explicit definitions for the lists have it allocated by a function, this will also allow us to expand the size of the board easily if we want
 */

