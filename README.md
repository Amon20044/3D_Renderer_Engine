# Cube Rendering with C and OpenGL

This code demonstrates rendering a rotating cube using C and OpenGL. The cube is illuminated with ambient, diffuse, and specular lighting.

## Libraries Used

- **GLFW:** A multi-platform library for creating windows with OpenGL contexts.
- **GLAD:** A library for loading OpenGL function pointers.
- **stdio.h:** Standard Input and Output library for printing errors.
- **math.h:** Standard C library for mathematical operations.

## Concepts

- **OpenGL Shading Language (GLSL):** Vertex and Fragment shaders are written in GLSL to perform lighting calculations.
- **Matrix Operations:** Functions are implemented for matrix operations such as multiplication, transpose, inversion, normalization, and projection.
- **Random Rotation Matrix:** Function generates a random rotation matrix for rotating the cube.

## Workflow

1. **Initialize GLFW:** Set OpenGL version and window parameters.
2. **Compile Shaders:** Compile vertex and fragment shaders and link them into a shader program.
3. **Generate Cube Data:** Define vertices and indices for the cube.
4. **Create Buffers:** Generate Vertex Array Object (VAO) and buffers for vertex and index data.
5. **Set Attribute Pointers:** Specify vertex attributes in the VAO.
6. **Render Loop:** Continuously render the cube while handling user input and updating transformations.
7. **Cleanup:** Delete allocated resources upon exit.

## Working

- The cube rotates around its center along all three axes.
- Ambient, diffuse, and specular lighting effects create a realistic appearance.
- The camera position and projection matrix provide perspective rendering.

## Algorithms

- **Matrix Multiplication:** Used to combine transformation matrices for model, view, and projection.
- **Matrix Transpose:** Required for normal matrix calculation in the vertex shader.
- **Matrix Inversion:** Used to compute the inverse of the model matrix for normal transformation.
- **Vector Normalization:** Normalize vectors to ensure consistent lighting calculations.
- **Projection Matrix:** Generates perspective projection for realistic rendering.

## Output

![Cube Rendering Output](https://github.com/Amon20044/3D_Renderer_Engine/assets/111745899/2cb6f09f-99de-475a-a672-563b98fdf515)

## Conclusion

This code provides a basic framework for rendering 3D objects with OpenGL, demonstrating fundamental concepts such as shaders, lighting, matrix transformations, and perspective projection. Further enhancements can be made to add textures, implement more complex lighting models, or render additional objects.
