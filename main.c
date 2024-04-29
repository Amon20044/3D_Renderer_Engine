    #include "stdio.h"
    #include "math.h"
    #include "glad/glad.h"
    #include "GLFW/glfw3.h"

    #define M_PI 3.14159265358979323846

    const char* vertexShaderSource = "#version 450 core\n"
    "layout (location=0) in vec3 aPos;\n"
    "layout (location=1) in vec3 aNormal;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"

    "mat4 transpose(in mat4 inMatrix) {\n"
    "    mat4 outMatrix;\n"
    "    for (int i = 0; i < 4; ++i)\n"
    "        for (int j = 0; j < 4; ++j)\n"
    "            outMatrix[j][i] = inMatrix[i][j];\n"
    "    return outMatrix;\n"
    "}\n"

    "mat4 inverse(in mat4 inMatrix) {\n"
    "    float a00 = inMatrix[0][0], a01 = inMatrix[0][1], a02 = inMatrix[0][2], a03 = inMatrix[0][3];\n"
    "    float a10 = inMatrix[1][0], a11 = inMatrix[1][1], a12 = inMatrix[1][2], a13 = inMatrix[1][3];\n"
    "    float a20 = inMatrix[2][0], a21 = inMatrix[2][1], a22 = inMatrix[2][2], a23 = inMatrix[2][3];\n"
    "    float a30 = inMatrix[3][0], a31 = inMatrix[3][1], a32 = inMatrix[3][2], a33 = inMatrix[3][3];\n"
    "    float b00 = a00 * a11 - a01 * a10;\n"
    "    float b01 = a00 * a12 - a02 * a10;\n"
    "    float b02 = a00 * a13 - a03 * a10;\n"
    "    float b03 = a01 * a12 - a02 * a11;\n"
    "    float b04 = a01 * a13 - a03 * a11;\n"
    "    float b05 = a02 * a13 - a03 * a12;\n"
    "    float b06 = a20 * a31 - a21 * a30;\n"
    "    float b07 = a20 * a32 - a22 * a30;\n"
    "    float b08 = a20 * a33 - a23 * a30;\n"
    "    float b09 = a21 * a32 - a22 * a31;\n"
    "    float b10 = a21 * a33 - a23 * a31;\n"
    "    float b11 = a22 * a33 - a23 * a32;\n"
    "    float det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;\n"
    "    if (det == 0.0)\n"
    "        return mat4(0.0);\n"
    "    mat4 outMatrix;\n"
    "    float invDet = 1.0 / det;\n"
    "    outMatrix[0][0] = (a11 * b11 - a12 * b10 + a13 * b09) * invDet;\n"
    "    outMatrix[0][1] = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;\n"
    "    outMatrix[0][2] = (a31 * b05 - a32 * b04 + a33 * b03) * invDet;\n"
    "    outMatrix[0][3] = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;\n"
    "    outMatrix[1][0] = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;\n"
    "    outMatrix[1][1] = (a00 * b11 - a02 * b08 + a03 * b07) * invDet;\n"
    "    outMatrix[1][2] = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;\n"
    "    outMatrix[1][3] = (a20 * b05 - a22 * b02 + a23 * b01) * invDet;\n"
    "    outMatrix[2][0] = (a10 * b10 - a11 * b08 + a13 * b06) * invDet;\n"
    "    outMatrix[2][1] = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;\n"
    "    outMatrix[2][2] = (a30 * b04 - a31 * b02 + a33 * b00) * invDet;\n"
    "    outMatrix[2][3] = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;\n"
    "    outMatrix[3][0] = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;\n"
    "    outMatrix[3][1] = (a00 * b09 - a01 * b07 + a02 * b06) * invDet;\n"
    "    outMatrix[3][2] = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;\n"
    "    outMatrix[3][3] = (a20 * b03 - a21 * b01 + a22 * b00) * invDet;\n"
    "    return outMatrix;\n"
    "}\n"

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

    "vec3 normalize(vec3 v) {\n"
    "    float inv_length = 1.0 / sqrt(dot(v, v));\n"
    "    return v * inv_length;\n"
    "}\n"

    "float max(float x, float y) {\n"
    "    return x > y ? x : y;\n"
    "}\n"

    "float pow(float x, float y) {\n"
    "    float result = 1.0;\n"
    "    for (int i = 0; i < int(y); ++i)\n"
    "        result *= x;\n"
    "    return result;\n"
    "}\n"

    "float dot(vec3 x, vec3 y) {\n"
    "    return x.x * y.x + x.y * y.y + x.z * y.z;\n"
    "}\n"

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

    void multiplyMat4(float* mat1, float* mat2, float* result) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[i * 4 + j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result[i * 4 + j] += mat1[i * 4 + k] * mat2[k * 4 + j];
                }
            }
        }
    }

    void transposeMat4(float* mat, float* result) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[i * 4 + j] = mat[j * 4 + i];
            }
        }
    }

    void inverseMat4(float* mat, float* result) {
        float det = mat[0] * (mat[5] * mat[10] - mat[6] * mat[9]) -
            mat[1] * (mat[4] * mat[10] - mat[6] * mat[8]) +
            mat[2] * (mat[4] * mat[9] - mat[5] * mat[8]);

        if (det == 0.0f) return; // Singular matrix

        float invDet = 1.0f / det;

        result[0] = (mat[5] * mat[10] - mat[6] * mat[9]) * invDet;
        result[1] = -(mat[1] * mat[10] - mat[2] * mat[9]) * invDet;
        result[2] = (mat[1] * mat[6] - mat[2] * mat[5]) * invDet;
        result[3] = 0.0f;

        result[4] = -(mat[4] * mat[10] - mat[6] * mat[8]) * invDet;
        result[5] = (mat[0] * mat[10] - mat[2] * mat[8]) * invDet;
        result[6] = -(mat[0] * mat[6] - mat[2] * mat[4]) * invDet;
        result[7] = 0.0f;

        result[8] = (mat[4] * mat[9] - mat[5] * mat[8]) * invDet;
        result[9] = -(mat[0] * mat[9] - mat[1] * mat[8]) * invDet;
        result[10] = (mat[0] * mat[5] - mat[1] * mat[4]) * invDet;
        result[11] = 0.0f;

        result[12] = -(mat[12] * result[0] + mat[13] * result[4] + mat[14] * result[8]);
        result[13] = -(mat[12] * result[1] + mat[13] * result[5] + mat[14] * result[9]);
        result[14] = -(mat[12] * result[2] + mat[13] * result[6] + mat[14] * result[10]);
        result[15] = 1.0f;
    }
    void normalizeVec3(float* vec) {
        float length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        vec[0] /= length;
        vec[1] /= length;
        vec[2] /= length;
    }

    // random rotation hoga idhar -- idhar Rodrigues formula use hoga
    void randomRotationMatrix(float angle, float* rotationMatrix, float rotationSpeedFactor) {
        // Normalize axis
        float axis[3] = { 1.0f, 3.0f, 1.0f };
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
        glEnable(GL_DEPTH_TEST);

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

        GLfloat angle = 0.0f;
        double lastTime = glfwGetTime();
        GLfloat speedFactor = 0.1f;
        //light ka source
        GLfloat lightPos[] = { 0.0f, 2.0f, 0.0f }; // Fixed light
        GLfloat viewPos[] = { 0.0f, 0.0f, 2.0f }; // Camera

        while (!glfwWindowShouldClose(window)) {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;
            lastTime = currentTime;
            // delta time lia hai for smooth rotation
            glClearColor(0.1f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shaderProgram);
            glBindVertexArray(VertexBufferArray);
            glDisable(GL_CULL_FACE);
            angle += deltaTime * speedFactor;

            GLfloat rotationMatrix[16];
            randomRotationMatrix(angle, rotationMatrix , speedFactor);

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
