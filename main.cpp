#include <glad/glad.h>   // Library to load the functions of OpenGL
#include <GLFW/glfw3.h>  // Library in C to manage windows and input

#include <print>

enum shaderType { SHADER, PROGRAM };

constexpr int MAJOR_VERSION{3};
constexpr int MINOR_VERSION{3};
constexpr int WIDTH{800};
constexpr int HEIGHT{600};
constexpr char TITLE[]{"Learn OpenGl"};

// Vertex shader source code
// No transformation done
constexpr const char* vertexShaderSource =
    "#version 330 core\n"                    // OpenGL version
    "layout (location = 0) in vec3 aPos;\n"  // Location of the input and the
                                             // type
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"  // Output of the
                                                             // vertex shader
    "}\0";

constexpr const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.4745f, 0.7215f, 1.0f);\n"
    "}\n\0";

// Callback function to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // The viewport defines the part of the window to draw and the first two
  // parameters are the coordinates of the bottom-left corner
  glViewport(0, 0, width, height);
}

// Manage the user's input
void processInput(GLFWwindow* window) {
  // Check the state of the key ESC if it's pressed
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);  // Close the window
}

bool checkCompilationOrLinking(const unsigned int& shader,
                               const shaderType& type) {
  int success;
  char infoLog[512];

  if (type == SHADER)
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  else if (type == PROGRAM)
    glGetProgramiv(shader, GL_LINK_STATUS, &success);

  if (!success) {
    if (type == SHADER) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::println("Failed to compile the shader: {}", infoLog);

    } else if (type == PROGRAM) {
      glGetProgramInfoLog(shader, 512, nullptr, infoLog);
      std::println("Failed to link the shader: {}", infoLog);
    }

    return false;
  }

  return true;
}

int main(void) {
  glfwInit();
  // Configuration of GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                 MAJOR_VERSION);  // Major version of OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                 MINOR_VERSION);  // Minor version of OpenGL
  glfwWindowHint(
      GLFW_OPENGL_PROFILE,
      GLFW_OPENGL_CORE_PROFILE);  // Subset of features of OpenGL (no
                                  // backward-compatibilities features)

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
  if (window == nullptr) {
    std::println("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }

  // Make the context of the window the MAIN context on the current thread,
  // otherwise GLAD could not load the functions
  glfwMakeContextCurrent(window);

  // Callback called when the window is resized
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /* glfwGetProcAddress get the address of OpenGL core and the loader load all
   * the functions with the address */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::println("Failed to load OpenGL functions");
    glfwTerminate();
    return -1;
  }

  std::array vertices{
      -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
  };

  /*
   * Vertex Array Object stores the state/configuration needed to tell OpenGL
   * how to interpret the vertex data in VBO
   */
  unsigned int vao;  // ID of the vertex array object
  // Generate one vertex array object and store its ID
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  /*
   * Vertex Buffer Object stores a large number of vertices in the GPU's memory.
   * The advantage is to send large batches to the GPU all at once.
   */
  unsigned int vbo;  // ID of the vertex buffer object
  // Generate one buffer object and store its ID in the second parameter
  glGenBuffers(1, &vbo);
  // Makes the buffer the current active buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // From now every buffer call on the GL_ARRAY_BUFFER affects vbo

  /*
   * Copy the data in the current buffer
   * The fourt parameter is essentuial because it specifies how the graphics
   * card manage the data:
   * - GL_STREAM_DRAW: the data is set only once and used by the GPU at most a
   * few times.
   * - GL_STATIC_DRAW: the data is set only once and used many times.
   * - GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
   */
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);

  /*
   * Instruct OpenGL how to interpret the vertex data:
   * - The first parameter tells which vertex we want to configure. The location
   * is based on the vertex attribute in the vertex shader
   * - The second specifies the size of the veretx attribute
   * - The third specifies the type of the data
   * - The fourth indicate to normalize
   * - The fifth indicates the stride so the space between consecutives vertex
   * attributes
   * - The sixth is the offset
   */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // Enable the vertex attribute and pass the location
  glEnableVertexAttribArray(0);

  // ========================== Build pipeline =========================
  // ----- Vertex shader ------
  unsigned int vertexShader;  // ID of the vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // Attach the shader source code
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  // Compile the shader
  glCompileShader(vertexShader);

  if (!checkCompilationOrLinking(vertexShader, shaderType::SHADER)) return -1;

  // ----- Fragment shader ------
  unsigned int fragmentShader;  // ID of the fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  if (!checkCompilationOrLinking(fragmentShader, shaderType::SHADER)) return -1;

  // ----- Shader program ------
  unsigned int shaderProgram;  // ID of the shader program
  shaderProgram = glCreateProgram();

  // Attach the shaders to the program
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  if (!checkCompilationOrLinking(shaderProgram, shaderType::PROGRAM)) return -1;

  // Delete the shader objects that are no longer used
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // =========================== Render loop ================================
  // Check if the window want to be closed
  // Every iteration of this loop is a frame
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // State-setting function to set the color of the screen
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // State-using function effectively clear the screen with the color setted
    // previously
    glClear(GL_COLOR_BUFFER_BIT);

    // Switches the active shader program
    glUseProgram(shaderProgram);

    /* Ensures the vertex attribute configuration matches the new shader's
     * expected layout, preventing potential rendering issues.
     */
    glBindVertexArray(vao);

    /* Start the pipeline:
     * - the first parameter indicates the primitive,
     * - the second indicates the starting index of the vbo,
     * - the third parameter indicates the number of vertices to draw.
     */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* GLFW by default use double buggering for rendering.
     * The buffer contains color values for each pixel in the windows.
     * - Front buffer represents the pixels visible on screen
     * - Back buffer is used for new drawing
     *
     * All drawings happens on the back buffer (not visible), when the rendering
     * is done then the buffers are swapped, back becomes the front and the
     * drawing is visible.
     *
     * Using a single buffer means that the rendering happens directly on
     * screen, so it can cause flickering or partial frames. */
    glfwSwapBuffers(window);

    // Polling to wait if any events are triggered
    glfwPollEvents();
  }

  // Deallocation of resources
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}