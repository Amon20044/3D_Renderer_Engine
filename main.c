#include "stdio.h"
#include "math.h"
#include "glad/glad.h"
#include "stdlib.h"
#include "GLFW/glfw3.h"

const char* vertexShaderSource = "#version 450 core\n"
"layout (location=0) in vec3 aPos;\n"
"layout (location=1) in vec3 aNormal;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
"    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"void main()\n"
"{\n"
"    // Ambient lighting\n"
"    float ambientStrength = 0.1;\n"
"    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);\n"
"    // Diffuse lighting\n"
"    vec3 lightDir = normalize(lightPos - FragPos);\n"
"    float diff = max(dot(Normal, lightDir), 0.0);\n"
"    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);\n"
"    // Specular lighting\n"
"    float specularStrength = 0.5;\n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    vec3 reflectDir = reflect(-lightDir, Normal);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);\n"
"    vec3 lighting = (ambient + diffuse + specular) * vec3(0.4, 0.6, 0.7);\n"
"    FragColor = vec4(lighting, 1.0);\n"
"}\n\0";

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// random rotation

float randomFloat() {
    return (float)rand() / RAND_MAX * 4.0f - 2.0f;
}
// Function to normalize a vector
void normalizeVec3(float* vec) {
    float length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    vec[0] /= length;
    vec[1] /= length;
    vec[2] /= length;
}

// Function to generate a random rotation matrix
void randomRotationMatrix(float angle, float* rotationMatrix, float rotationSpeedFactor) {
    // Normalize axis
    float axis[3] = { 2.0f, 3.0f, 1.0f };
    normalizeVec3(axis);

    // Calculate sines and cosines of the angle
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);
    float oneMinusCosAngle = 1.0f - cosAngle;

    // Fill in the rotation matrix
    rotationMatrix[0] = axis[0] * axis[0] * oneMinusCosAngle + cosAngle;
    rotationMatrix[1] = axis[0] * axis[1] * oneMinusCosAngle - axis[2] * sinAngle;
    rotationMatrix[2] = axis[0] * axis[2] * oneMinusCosAngle + axis[1] * sinAngle;
    rotationMatrix[3] = 0.0f;

    rotationMatrix[4] = axis[1] * axis[0] * oneMinusCosAngle + axis[2] * sinAngle;
    rotationMatrix[5] = axis[1] * axis[1] * oneMinusCosAngle + cosAngle;
    rotationMatrix[6] = axis[1] * axis[2] * oneMinusCosAngle - axis[0] * sinAngle;
    rotationMatrix[7] = 0.0f;

    rotationMatrix[8] = axis[2] * axis[0] * oneMinusCosAngle - axis[1] * sinAngle;
    rotationMatrix[9] = axis[2] * axis[1] * oneMinusCosAngle + axis[0] * sinAngle;
    rotationMatrix[10] = axis[2] * axis[2] * oneMinusCosAngle + cosAngle;
    rotationMatrix[11] = 0.0f;

    rotationMatrix[12] = 0.0f;
    rotationMatrix[13] = 0.0f;
    rotationMatrix[14] = 0.0f;
    rotationMatrix[15] = 1.0f;
}




int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    srand(time(NULL));
    GLFWwindow* window = glfwCreateWindow(800, 800, "Cube Rendering", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, 800, 800);
    glClearColor(0.1f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    };

    GLuint indices[] = {
        0, 1, 2, // Front face
        2, 3, 0,
        4, 5, 6, // Back face
        6, 7, 4,
        0, 4, 7, // Left face
        7, 3, 0,
        1, 5, 6, // Right face
        6, 2, 1,
        3, 2, 6, // Top face
        6, 7, 3,
        0, 1, 5, // Bottom face
        5, 4, 0,
    };

    GLuint VertexBufferObject, VertexBufferArray, ElementBufferObject;
    glGenVertexArrays(1, &VertexBufferArray);
    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);

    glBindVertexArray(VertexBufferArray);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLfloat angle = 1.0f;
    double lastTime = glfwGetTime();
    GLfloat speedFactor = 0.001f;
    // Set up light source properties
    GLfloat lightPos[] = { 1.0f, 1.0f, 2.0f };
    GLfloat viewPos[] = { 0.0f, 0.0f, 3.0f }; // Camera position

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClearColor(0.1f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(shaderProgram);
        glBindVertexArray(VertexBufferArray);

        angle += deltaTime;

        // Rotation matrix around Z-axis
        GLfloat rotationMatrix[16];
        randomRotationMatrix(angle, rotationMatrix, speedFactor);

        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, rotationMatrix);

        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, rotationMatrix);

        GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, rotationMatrix);

        GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightPosLoc, 1, lightPos);

        GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        glUniform3fv(viewPosLoc, 1, viewPos);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VertexBufferArray);
    glDeleteBuffers(1, &VertexBufferObject);
    glDeleteBuffers(1, &ElementBufferObject);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Exiting...\n");
    return 0;
}
