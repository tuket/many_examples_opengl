#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>

#include <camera.hpp>
#include <utils.hpp>

using namespace std;

const unsigned FRAMES_PER_SEC = 60;
const unsigned TICKS_PER_SEC = 1000;
const unsigned TICKS_PER_FRAME = TICKS_PER_SEC / FRAMES_PER_SEC;

vector<string> attribNames;
vector<string> textures;

// uniform names
struct UniformNames
{
    const static string transfMat;
};

const string UniformNames::transfMat = "transfMat";

void initNames()
{
    attribNames.push_back("pos");
    attribNames.push_back("texCoord0");

    textures.push_back("texture0");

}

int main()
{

    initNames();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window =
            SDL_CreateWindow
            (
                "example 9",
                100, 100,
                800, 600,
                SDL_WINDOW_OPENGL
            );

    //SDL_GLContext context =
    SDL_GL_CreateContext(window);

    if(!gladLoadGL()) {
        printf("glad error!\n");
        exit(-1);
    }
    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    if (GLVersion.major < 2) {
        printf("Your system doesn't support OpenGL >= 2!\n");
        return -1;
    }
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.1, 0.1, 0.1, 1);

    // buffers
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    GLfloat verts[] =
    {
        -0.5, 0.5,      0, 1,
        0.5, 0.5,       1, 1,
        0.5, -0.5,      1, 0,
        -0.5, -0.5,     0, 0
    };

    GLuint inds[] =
    {
        3, 0, 2, 1
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

    // shaders

    GLuint shadProgram = glCreateProgram();

    glBindAttribLocation(shadProgram, 0, attribNames[0].c_str());
    glBindAttribLocation(shadProgram, 1, attribNames[1].c_str());

    bool programOk = makeShaderProgram
                    (
                        shadProgram,
                        "examples/ex7/shaders/vert.glsl",
                        "examples/ex7/shaders/frag.glsl"
                    );

    if(!programOk)
    {
        cout << "error in the shader program" << endl;
        exit(1);
    }

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer   // + pos
            (
                0,          // attrib index
                2,          // number of elements
                GL_FLOAT,
                GL_FALSE,   // normalize fixed point
                4 * sizeof(GLfloat),          // stride
                (void*) 0   // first element pos
             );

    glVertexAttribPointer   // + pos
            (
                1,          // attrib index
                2,          // number of elements
                GL_FLOAT,
                GL_FALSE,   // normalize fixed point
                4 * sizeof(GLfloat),          // stride
                (void*) (2*sizeof(GLfloat))   // first element pos
             );

    // uniforms & textures
    glUseProgram(shadProgram);
    GLuint texture;
    glGenTextures(1, &texture);
    GLuint texUnif = glGetUniformLocation(shadProgram, textures[0].c_str());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int w, h, comp;
    GLubyte* imgData = loadImage("img/vamp.png", w, h, comp);
    glTexImage2D
            (
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                w, h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                imgData
            );
    freeImage(imgData);
    glUniform1i(texUnif, 0);

    GLuint transfMathUnif = glGetUniformLocation(shadProgram, UniformNames::transfMat.c_str());
    Camera cam;
    cam.setPosition(glm::vec3(0, 0, 2));
    glm::mat4 mat = cam.getTransfMat();
    glUniformMatrix4fv(transfMathUnif, 1, GL_FALSE, glm::value_ptr(mat));

    bool continueExec = true;
    int ticks = SDL_GetTicks();
    while(continueExec)
    {
        int newTicks = static_cast<int>(SDL_GetTicks());

        // handle events
        SDL_Event event;
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
        {
            continueExec = false;
        }
        else if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    continueExec = false;
                break;
            }
        }
        else if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
            }

        }

        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        bool forwardPressed = keyState[SDL_SCANCODE_W];
        bool backwardPressed = keyState[SDL_SCANCODE_S];
        bool leftPressed = keyState[SDL_SCANCODE_A];
        bool rightPressed = keyState[SDL_SCANCODE_D];
        bool latLeftPressed = keyState[SDL_SCANCODE_Q];
        bool latRightPressed= keyState[SDL_SCANCODE_E];

        // cam move
        const float CAM_SPEED = 0.02;
        glm::vec3 disp;
        float h = 0;
        if(forwardPressed)
        {
            disp = glm::vec3(0, 0, -1);
        }
        else if(backwardPressed)
        {
            disp = glm::vec3(0, 0, 1);
        }
        if(leftPressed)
        {
            h = degToRad(0.5);
        }
        else if(rightPressed)
        {
            h = degToRad(-0.5);
        }
        if(latLeftPressed)
        {
            disp += glm::vec3(-1, 0, 0);
        }
        else if(latRightPressed)
        {
            disp += glm::vec3(1, 0, 0);
        }
        disp = disp / disp.length();
        disp *= CAM_SPEED;
        float heading = cam.getHeading();
        heading += h;
        cam.translateLocal(disp);
        cam.setHeading(heading);
        mat = cam.getTransfMat();
        glUniformMatrix4fv(transfMathUnif, 1, GL_FALSE, glm::value_ptr(mat));

        // draw stuff
        glClear(GL_COLOR_BUFFER_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glUseProgram(shadProgram);

        glDrawElements(GL_TRIANGLE_STRIP, sizeof(inds)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        int ticksDiff = newTicks - ticks;
        ticks = newTicks;

        int waitTicks = static_cast<int>(TICKS_PER_FRAME) - (SDL_GetTicks() - newTicks) ;

        if(waitTicks > 0) SDL_Delay(static_cast<unsigned>(waitTicks));

    }

}
