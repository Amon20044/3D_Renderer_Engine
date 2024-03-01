#include "stdio.h";
#include "glad/glad.h";
#include "GLFW/glfw3.h";

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "MyFirstOpenGL_MA202Project", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	// swapping colour - in background instance - in front and back buffer
	glClearColor(0.1f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	glViewport(0, 0, 1200, 800);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	printf("Amon ! its easy - its just matter of time");
	return 0;
}