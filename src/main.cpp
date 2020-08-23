#include "common.h"

GLFWwindow *window;

Mesh *mesh;

/* for view control */
float verticalAngle = -2.13668;
float horizontalAngle = 0.0107599;
float initialFoV = 45.0f;
float speed = 5.0f;
float mouseSpeed = 0.005f;
float nearPlane = 0.01f, farPlane = 1000.f;

mat4 model, view, projection;
vec3 eyePoint = vec3(8.877738, 5.290346, -0.256238);
vec3 eyeDirection =
    vec3(sin(verticalAngle) * cos(horizontalAngle), cos(verticalAngle),
         sin(verticalAngle) * sin(horizontalAngle));
vec3 up = vec3(0.f, 1.f, 0.f);

// pbr test
vec3 lightPositions[4] = {vec3(3.0f, 3.0f, 3.0f), vec3(3.0f, 3.0f, -3.0f),
                          vec3(3.0f, -3.0f, 3.0f), vec3(-3.0f, 3.0f, 3.0f)};

vec3 lightColors[4] = {vec3(10.f, 10.f, 10.f), vec3(20.f, 20.f, 20.f),
                       vec3(30.f, 30.f, 30.f), vec3(40.f, 40.f, 40.f)};

// test
vector<Point> pts;
GLuint pointShader;
GLint uniPointM, uniPointV, uniPointP;

void computeMatricesFromInputs();
void keyCallback(GLFWwindow *, int, int, int, int);

void initGL();
void initOthers();
void initMatrix();
void initTexture();
void releaseResource();

int main(int argc, char **argv) {
  initGL();
  initOthers();

  for (size_t i = 0; i < 4; i++) {
    Point p;
    p.pos = lightPositions[i];
    p.color = vec3(1.f);
    pts.push_back(p);
  }

  // prepare mesh data
  mesh = new Mesh("./mesh/sphereQuad.obj", true);

  initTexture();
  initMatrix();

  // a rough way to solve cursor position initialization problem
  // must call glfwPollEvents once to activate glfwSetCursorPos
  // this is a glfw mechanism problem
  glfwPollEvents();
  glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    // reset
    glClearColor(0.f, 0.f, 0.4f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // view control
    computeMatricesFromInputs();

    // It is better to always use transform matrix
    // to move, rotate and scale objects.
    // This can avoid updating vertex buffers.
    for (int r = 0; r < 1; r++) {
      for (int c = 0; c < 1; c++) {
        mat4 tempModel = translate(mat4(1.f), vec3(2.f * r, 0.f, 2.f * c));
        tempModel = scale(tempModel, vec3(3.f, 3.f, 3.f));

        mesh->draw(tempModel, view, projection, eyePoint, lightColors,
                   lightPositions, 12, 13, 14, 15, 16);
      }
    }

    glUseProgram(pointShader);
    glUniformMatrix4fv(uniPointM, 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(uniPointV, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(uniPointP, 1, GL_FALSE, value_ptr(projection));
    drawPoints(pts);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  releaseResource();

  return EXIT_SUCCESS;
}

void computeMatricesFromInputs() {
  // glfwGetTime is called only once, the first time this function is called
  static float lastTime = glfwGetTime();

  // Compute time difference between current and last frame
  float currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);

  // Get mouse position
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  // Reset mouse position for next frame
  glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

  // Compute new orientation
  // The cursor is set to the center of the screen last frame,
  // so (currentCursorPos - center) is the offset of this frame
  horizontalAngle += mouseSpeed * float(xpos - WINDOW_WIDTH / 2.f);
  verticalAngle += mouseSpeed * float(-ypos + WINDOW_HEIGHT / 2.f);

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  vec3 direction =
      vec3(sin(verticalAngle) * cos(horizontalAngle), cos(verticalAngle),
           sin(verticalAngle) * sin(horizontalAngle));

  // Right vector
  vec3 right = vec3(cos(horizontalAngle - 3.14 / 2.f), 0.f,
                    sin(horizontalAngle - 3.14 / 2.f));

  // new up vector
  vec3 newUp = cross(right, direction);

  // Move forward
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    eyePoint += direction * deltaTime * speed;
  }
  // Move backward
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    eyePoint -= direction * deltaTime * speed;
  }
  // Strafe right
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    eyePoint += right * deltaTime * speed;
  }
  // Strafe left
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    eyePoint -= right * deltaTime * speed;
  }

  // float FoV = initialFoV;
  projection = perspective(initialFoV, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT,
                           nearPlane, farPlane);
  // Camera matrix
  view = lookAt(eyePoint, eyePoint + direction, newUp);

  // For the next frame, the "last time" will be "now"
  lastTime = currentTime;
}

void keyCallback(GLFWwindow *keyWnd, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(keyWnd, GLFW_TRUE);
      break;
    }
    case GLFW_KEY_F: {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    }
    case GLFW_KEY_L: {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    }
    case GLFW_KEY_I: {
      std::cout << "eyePoint: " << to_string(eyePoint) << '\n';
      std::cout << "verticleAngle: " << fmod(verticalAngle, 6.28f) << ", "
                << "horizontalAngle: " << fmod(horizontalAngle, 6.28f) << endl;
      break;
    }
    default:
      break;
    }
  }
}

void initGL() { // Initialise GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    exit(EXIT_FAILURE);
  }

  // without setting GLFW_CONTEXT_VERSION_MAJOR and _MINOR，
  // OpenGL 1.x will be used
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // must be used if OpenGL version >= 3.0
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PBR", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to open GLFW window." << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  /* Initialize GLEW */
  // without this, glGenVertexArrays will report ERROR!
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST); // must enable depth test!!

  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(20);

  glPatchParameteri(GL_PATCH_VERTICES, 4);
}

void initOthers() {
  FreeImage_Initialise(true);

  pointShader = buildShader("./shader/vsPoint.glsl", "./shader/fsPoint.glsl");
  glUseProgram(pointShader);
  uniPointM = myGetUniformLocation(pointShader, "M");
  uniPointV = myGetUniformLocation(pointShader, "V");
  uniPointP = myGetUniformLocation(pointShader, "P");
}

void initMatrix() {
  model = translate(mat4(1.f), vec3(0.f, 0.f, 0.f));
  view = lookAt(eyePoint, eyeDirection, up);
  projection = perspective(initialFoV, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT,
                           nearPlane, farPlane);
}

void initTexture() {
  mesh->setTexture(mesh->tboBase, 12, "./res/stone_base.jpg", FIF_JPEG);
  mesh->setTexture(mesh->tboNormal, 13, "./res/stone_normal.jpg", FIF_JPEG);
  mesh->setTexture(mesh->tboAO, 14, "./res/stone_ao.jpg", FIF_JPEG);
  mesh->setTexture(mesh->tboRough, 15, "./res/stone_roughness.jpg", FIF_JPEG);
  mesh->setTexture(mesh->tboHeight, 16, "./res/stone_height.jpg", FIF_JPEG);
}

void releaseResource() {
  glfwTerminate();
  FreeImage_DeInitialise();

  delete mesh;
}
