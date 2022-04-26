#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "shader.h"
// #include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
unsigned int loadTexture(const char *path);
void declare_vertices();

void renderCube(unsigned int& VAO);
void renderQuad(unsigned int& quadVAO);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool keys[1024];  // key input array

GLfloat deltaTime = 0.0f;   // 当前帧遇上一帧的时间差
GLfloat lastFrame = 0.0f;   // 上一帧的时间
GLfloat lastX = 400, lastY = 300;     // for mouse input
bool firstMouse = true;
GLfloat fov = 45.0f;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

    // glfw window creation
    // --------------------
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BloomProject", monitor, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the required callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Enables Multisampling
    //glEnable(GL_MULTISAMPLE);

    // Enables Cull Facing
    //glEnable(GL_CULL_FACE);
    // Keeps front faces
    //glCullFace(GL_FRONT);
    // Uses counter clock-wise standard
    //glFrontFace(GL_CCW);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shaders/vertex.glsl", "shaders/frag.glsl");
    Shader lampShader("shaders/lightVertex.glsl", "shaders/lightFrag.glsl");
    Shader blurShader("shaders/screenVertex.glsl", "shaders/blurFrag.glsl");
    //Shader screenShader("shaders/screenVertex.glsl", "shaders/hdrFrag.glsl");
    Shader bokehShader("shaders/screenVertex.glsl", "shaders/blurFrag.glsl");
    Shader bloomFinalShader("shaders/screenVertex.glsl", "shaders/bloomFrag.glsl");

    Shader quadShader("shaders/screenVertex.glsl", "shaders/quadFrag.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-0.5f,  0.0f, -2.0f),
            glm::vec3( -2.0f,  0.0f, -3.0f)
    };
    glm::vec3 pointLightColors[] = {
            glm::vec3(0.25f, 0.25f, 0.6f),
            glm::vec3(0.4f, 0.1f, 0.5f),
            //glm::vec3(1.0f, 1.0, 0.0),
            glm::vec3(2.5f, 2.5f, 10.0f),
            glm::vec3(0.8f, 10.2f, 1.0f)
    };

    unsigned int cubeVBO, cubeVAO, planeVAO, planeVBO, quadVAO, quadVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    // cube
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    float planeVertices[] = {
            // positions         // normal coords     // texture Coords
            20.0f, -4.0f,  20.0f,  0.0f,  1.0f,  0.0f,   2.0f, 0.0f,
            -20.0f, -4.0f,  20.0f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
            -20.0f, -4.0f, -20.0f,  0.0f,  1.0f,  0.0f,   0.0f, 2.0f,

            20.0f, -4.0f,  20.0f,  0.0f,  1.0f,  0.0f,   2.0f, 0.0f,
            -20.0f, -4.0f, -20.0f,  0.0f,  1.0f,  0.0f,   0.0f, 2.0f,
            20.0f, -4.0f, -20.0f,  0.0f,  1.0f,  0.0f,   2.0f, 2.0f
    };
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
            /* // small screen
             -0.3f,  1.0f,  0.0f, 1.0f,
             -0.3f,  0.7f,  0.0f, 0.0f,
             0.3f,  0.7f,  1.0f, 0.0f,

             -0.3f,  1.0f,  0.0f, 1.0f,
             0.3f,  0.7f,  1.0f, 0.0f,
             0.3f,  1.0f,  1.0f, 1.0f */
    };
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    // load textures from folder
    unsigned int texture1 = loadTexture("resources/container2.png");
    unsigned int texture2 = loadTexture("resources/container2_specular.png");
    unsigned int floorTexture = loadTexture("resources/pexels-scott-webb-2117937.jpg");

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------

    // configure floating point framebuffer
    // ------------------------------------
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    // initialize color attachment textures
    unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }

    // create a renderbuffer
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // add renderbuffer to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // set it to draw the two buffers together
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    // check if framebuffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate a downsampleFBO
    unsigned int downsampleFBO;
    glGenFramebuffers(1, &downsampleFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBO);
    unsigned int downsampleColorBuffer;
    glGenTextures(1, &downsampleColorBuffer);
    glBindTexture(GL_TEXTURE_2D, downsampleColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH/2, SCR_HEIGHT/2, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downsampleColorBuffer, 0);

    // create two pingpong color FBOs, each with a distinct texture
    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffers);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i], 0);
    }

    // generate a second downsampleFBO
    unsigned int downsampleFBO_2;
    glGenFramebuffers(1, &downsampleFBO_2);
    glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBO_2);
    unsigned int downsampleColorBuffer_2;
    glGenTextures(1, &downsampleColorBuffer_2);
    glBindTexture(GL_TEXTURE_2D, downsampleColorBuffer_2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH/4, SCR_HEIGHT/4, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downsampleColorBuffer_2, 0);

    unsigned int second_pass_pingpongFBO[2];
    unsigned int second_pass_pingpongBuffers[2];
    glGenFramebuffers(2, second_pass_pingpongFBO);
    glGenTextures(2, second_pass_pingpongBuffers);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, second_pass_pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, second_pass_pingpongBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, second_pass_pingpongBuffers[i], 0);
    }

    unsigned int third_pass_FBO;
    unsigned int third_pass_ColorBuffer;
    glGenFramebuffers(1, &third_pass_FBO);
    glGenTextures(1, &third_pass_ColorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, third_pass_FBO);
    glBindTexture(GL_TEXTURE_2D, third_pass_ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, third_pass_ColorBuffer, 0);

    blurShader.use();
    blurShader.setInt("screenTexture", 0);
    bloomFinalShader.use();
    bloomFinalShader.setInt("screenTexture", 0);
    bloomFinalShader.setInt("bloomTexture", 1);
    bloomFinalShader.setInt("bloomTexture2", 2);
    bloomFinalShader.setInt("bloomTexture3", 3);

    quadShader.use();
    quadShader.setInt("screenTexture", 0);

    // note that we're translating the scene in the reverse direction of where we want to move
    //   view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // bind fbo
    //glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    std::cout << "\n===== FBO STATUS =====\n";

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    std::cout << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << std::endl;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time of each frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // key input
        do_movement();
        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render container
        ourShader.use();

        // bind textures to corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, floorTexture);

        // 给fragment shader里的lightPos, viewPos uniform赋值

        ourShader.setVec3("viewPos", cameraPos);
        ourShader.setInt("material.diffuse", 0);
        ourShader.setInt("material.specular", 1);
        ourShader.setFloat("material.shininess", 32.0f);

        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.2f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 ambientColor = glm::vec3(0.1f);
        glm::vec3 diffuseColor = glm::vec3(0.5f);
        glm::vec3 ambientColor2 = glm::vec3(0.05f);
        // set values for direct light
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
        ourShader.setVec3("dirLight.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // set values for point light
        ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        ourShader.setFloat("pointLights[0].constant", 1.0f);
        ourShader.setFloat("pointLights[1].constant", 1.0f);
        ourShader.setFloat("pointLights[2].constant", 1.0f);
        ourShader.setFloat("pointLights[3].constant", 1.0f);
        ourShader.setFloat("pointLights[0].linear", 0.09f);
        ourShader.setFloat("pointLights[1].linear", 0.09f);
        ourShader.setFloat("pointLights[2].linear", 0.09f);
        ourShader.setFloat("pointLights[3].linear", 0.09f);
        ourShader.setFloat("pointLights[0].quadratic", 0.032);
        ourShader.setFloat("pointLights[1].quadratic", 0.032);
        ourShader.setFloat("pointLights[2].quadratic", 0.032);
        ourShader.setFloat("pointLights[3].quadratic", 0.032);

        ourShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1,  pointLightColors[0].y * 0.1,  pointLightColors[0].z * 0.1);
        ourShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1,  pointLightColors[1].y * 0.1,  pointLightColors[1].z * 0.1);
        ourShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1,  pointLightColors[2].y * 0.1,  pointLightColors[2].z * 0.1);
        ourShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1,  pointLightColors[3].y * 0.1,  pointLightColors[3].z * 0.1);
        ourShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
        ourShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x,  pointLightColors[0].y,  pointLightColors[0].z);
        ourShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x,  pointLightColors[0].y,  pointLightColors[0].z);
        ourShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x,  pointLightColors[0].y,  pointLightColors[0].z);
        ourShader.setVec3("pointLights[0].specular", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
        ourShader.setVec3("pointLights[1].specular", pointLightColors[1].x,  pointLightColors[0].y,  pointLightColors[0].z);
        ourShader.setVec3("pointLights[2].specular", pointLightColors[2].x,  pointLightColors[0].y,  pointLightColors[0].z);
        ourShader.setVec3("pointLights[3].specular", pointLightColors[3].x,  pointLightColors[0].y,  pointLightColors[0].z);

        // set values for spotlight
        ourShader.setVec3("spotLight.position", lightPos.x, lightPos.y, lightPos.z);
        ourShader.setVec3("spotLight.direction", cameraFront.x, cameraFront.y, cameraFront.z);
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        ourShader.setVec3("spotLight.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
        ourShader.setVec3("spotLight.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.09f);
        ourShader.setFloat("spotLight.quadratic", 0.032);

        float timeValue = glfwGetTime();
        std::cout << timeValue << std::endl;

        // set view, projection and model matrices for cube rendering
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        //int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        // render cubes
        for (unsigned int i=0; i<sizeof(cubePositions); i++)
        {
            // model matrix
            glm:: mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            renderCube(cubeVAO);
        }

        // set floor texture and draw floor
        ourShader.setMat4("model", glm::mat4(1.0f));
        ourShader.setInt("material.diffuse", 2);
        ourShader.setInt("material.specular", 2);
        renderQuad(planeVAO);

        //std::cout << glGetError() << std::endl;

        // set lamp shader
        lampShader.use();
        // uses a new model matrix for light source, and the same view and projection matrices
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);

        // draw point lights
        for (unsigned int i=0; i<4; i++)
        {
            glm:: mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.3f));
            lampShader.setMat4("model", model);
            lampShader.setVec3("lightColor", pointLightColors[i].x, pointLightColors[i].y, pointLightColors[i].z);
            renderCube(cubeVAO);
        }

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(0.25f, 0.5f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        //std::cout << glGetError() << std::endl;
        // error checking
        //glDebugMessageCallback(MessageCallback, 0);
/*
        blurShader.use();
        //blurShader.setInt("horizontal", false);
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        std::cout << glGetError() << std::endl; */


        // downsample the bright texture
        glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBO);
        glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT / 2);
        quadShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
        renderQuad(quadVAO);

        bool horizontal = true, first_iteration = true;
        int amount = 6;
        blurShader.use();
/*
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[!horizontal]);
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
*/
        // draw the first blurred texture
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            blurShader.setInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]);
            if (first_iteration) {
                glBindTexture(GL_TEXTURE_2D, downsampleColorBuffer);
                first_iteration = false;
            }
            else {
                glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);
            }
            renderQuad(quadVAO);
            horizontal = !horizontal;
        }

        // second downsampled texture
        glBindFramebuffer(GL_FRAMEBUFFER, downsampleFBO_2);
        glViewport(0, 0, SCR_WIDTH / 4, SCR_HEIGHT / 4);
        quadShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
        renderQuad(quadVAO);

        //std::cout << horizontal << std::endl;

        /* glBindFramebuffer(GL_FRAMEBUFFER, second_pass_pingpongFBO[!horizontal]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);	// use the color attachment texture as the texture of the quad plane
        renderQuad(quadVAO); */

        // second blurred layer
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        first_iteration = true;
        bool horizontal_2 = true;
        int amount_2 = 6;
        for (unsigned int i = 0; i < amount_2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, second_pass_pingpongFBO[horizontal_2]);
            blurShader.setInt("horizontal", horizontal_2);
            glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]);
            if (first_iteration) {
                glBindTexture(GL_TEXTURE_2D, downsampleColorBuffer_2);
                first_iteration = false;
            }
            else {
                glBindTexture(GL_TEXTURE_2D, second_pass_pingpongFBO[!horizontal_2]);
            }
            renderQuad(quadVAO);
            horizontal_2 = !horizontal_2;
        }
/*
        // third blurred layer
        glBindFramebuffer(GL_FRAMEBUFFER, third_pass_FBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, second_pass_pingpongFBO[!horizontal_2]);	// use the color attachment texture as the texture of the quad plane
        renderQuad(quadVAO);
*/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        // blend the textures together
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        bloomFinalShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);
        //glBindTexture(GL_TEXTURE_2D, downsampleColorBuffer);
        glActiveTexture(GL_TEXTURE2);
        //glBindTexture(GL_TEXTURE_2D, second_pass_pingpongBuffers[!horizontal_2]);
        glBindTexture(GL_TEXTURE_2D, downsampleColorBuffer_2);
        //glActiveTexture(GL_TEXTURE3);
        //glBindTexture(GL_TEXTURE_2D, third_pass_ColorBuffer);
        bloomFinalShader.setFloat("exposure", 0.5);
        renderQuad(quadVAO);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //std::cout << glGetError() << std::endl;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // gamma correction
    // glEnable(GL_FRAMEBUFFER_SRGB);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &quadVBO);
    //glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //  glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void do_movement()
{
    GLfloat cameraSpeed = 5.0f * deltaTime;       // higher the framerate (lower delta time), lower the speed
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;   // right vector
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标的范围是从下往上的
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

//Define cursor callback function
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset / 1.5f;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 90.0f)
        fov = 90.0f;
}

unsigned int loadTexture(const char *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    std::cout << " - Image color channels: " << nrChannels << std::endl;
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        std::cout  << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}

void declare_vertices()
{

}

void renderCube(unsigned int& VAO)
{
    // render Cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void renderQuad(unsigned int& quadVAO)
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// update color
/*
float redValue = sin(timeValue) / 4.0 + 0.75;
int vertexColorLocation = glGetUniformLocation(ourShader.ID, "altColor");
glUniform4f(vertexColorLocation, redValue, 0.6f, 0.5f, 1.0f); */

// self-rotating camera
/* view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            GLfloat radius = 20.0f;
            GLfloat camX = sin(glfwGetTime()) * radius;
            GLfloat camZ = cos(glfwGetTime()) * radius;
            view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); */

// square
/*float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
}; */

