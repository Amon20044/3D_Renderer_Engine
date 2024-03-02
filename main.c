#include "stdio.h";
#include "math.h"
#include "glad/glad.h";
#include "GLFW/glfw3.h";

const char* vertexShaderSource = "#version 450 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x , aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.4f , 0.6f, 0.7f, 1.0f);\n"
"}\n\0";



//Adding some basic callbacks
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
// callbacks

void triangle() {
	

}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// telling glfw about using the CORE profile and mordern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// making triangle
	// vertices coordinates (x,y,z) format
	GLfloat vertices[] = {
		-0.5f, -0.5f * 1.73205081 / 3,0.0f, 
		0.5f, -0.5f * 1.73205081 / 3,0.0f,
		0.0f, 0.5f * 1.73205081* 2 / 3,0.0f,
	};
	// create a GLFW window of 1200 by 800 px
	GLFWwindow* window = glfwCreateWindow(1200, 800, "MyFirstOpenGL_MA202Project", NULL, NULL);
	if (window == NULL) {
		// error checking
		printf("Failed to create window\n");
		glfwTerminate();
		return -1;
	}
	//setting callback event during error handling
	glfwSetErrorCallback(error_callback);
	// geting window context so that we can use it current window and change the data in it
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetKeyCallback(window, key_callback);
	
    glViewport(0, 0, 1200, 800);

	// swapping colour - in background instance - in front and back buffer
	glClearColor(0.1f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);




	// create vertex shader object and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile the vertex shader object into machine code
	glCompileShader(vertexShader);

	// create vertex shader fragment and get reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// attach fragment shader source to fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// compile fragment shader object 
	glCompileShader(fragmentShader);

	// create shader program object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// attaching vertex and fragment objects to shaderprogram object
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up / Link all shaders together
	glLinkProgram(shaderProgram);

	// deleting useless stuff for optimization
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);





	//creating reference container for vertex buffer object and vertex buffer array
	GLuint VertexBufferObject, VertexBufferArray;

	//generate the VAO (vertex buffer array object / vertex array object) and VBO with 1 object each
	glGenVertexArrays(1, &VertexBufferArray);
	glGenBuffers(1, &VertexBufferObject); // specifying number of objects to be rendered and poiting it to the reference

	// makje the VAO the current VAO by binding it
	glBindVertexArray(VertexBufferArray);
	// bind the VBO specifyoing it is a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);


	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //specify for optimization


	// configure the vertex attribute so that openGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enable the vertex attrivute so that opengl knows how to use it
	glEnableVertexAttribArray(0);

	// bind both the VBO and VAO to 0 so that we dont accidentally modify the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//making triangle

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// tell opengl which shader program we want to use
		glUseProgram(shaderProgram);
		// bind VAO so opengl knows how to use it
		glBindVertexArray(VertexBufferArray);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window); //updation of window with generated buffers 
		// takae care of all glfw events
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VertexBufferArray);
	glDeleteBuffers(1, &VertexBufferObject);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	printf("Amon ! its easy - its just matter of time");
	return 0;
}