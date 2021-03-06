#include <iostream>
#include <vector>
#include <SDL.h>
#include <glad/glad.h>
#include "utils.hpp"

using namespace std;

const unsigned FRAMES_PER_SEC = 60;
const unsigned TICKS_PER_SEC = 1000;
const unsigned TICKS_PER_FRAME = TICKS_PER_SEC / FRAMES_PER_SEC;

vector<string> attribNames;

void initAttribNames()
{
    attribNames.push_back("pos");
    attribNames.push_back("color");
}

int main(int argc, char** argv)
{

    initAttribNames();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window =
            SDL_CreateWindow
            (
                "example 3",
                100, 100,
                800, 600,
                SDL_WINDOW_OPENGL
            );

    //SDL_GLContext context =
            SDL_GL_CreateContext(window);

    if(!gladLoadGL()) {
        cout << "glad error!" << endl;
        exit(-1);
    }
    cout << "OpenGL  " << GLVersion.major << "." << GLVersion.minor << endl;
    if (GLVersion.major < 2) {
        cout << "Your system doesn't support OpenGL >= 2!\n" << endl;
        return -1;
    }
    cout << "OpenGL " << glGetString(GL_VERSION)
         << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    glClearColor(0, 0, 0, 1);

    // buffers
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    GLfloat verts[] =
    {
        // pos          color
        0, 0.5f,        1, 0, 0,
        0.5f, -0.5f,    0, 1, 0,
        -0.5f, -0.5f,   0, 0, 1
    };

    GLfloat verts1[] =
    {
        // pos          color
        0, 0.5f,        0, 1, 0,
        0.5f, -0.5f,    0, 0, 1,
        -0.5f, -0.5f,   1, 0, 0
    };

    GLfloat verts2[] =
    {
        // pos          color
        0, 0.5f,        0, 0, 1,
        0.5f, -0.5f,    1, 0, 0,
        -0.5f, -0.5f,   0, 1, 0
    };

    GLuint inds[] = {0, 1, 2, 0};

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_DYNAMIC_DRAW);

    // shaders
    GLuint shaderProg = glCreateProgram();

    glBindAttribLocation(shaderProg, 0, attribNames[0].c_str());
    glBindAttribLocation(shaderProg, 1, attribNames[1].c_str());

    bool programOk = makeShaderProgram
                        (
                            shaderProg,
                            "examples/ex2/shaders/vert.glsl",
                            "examples/ex2/shaders/frag.glsl"
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
                5 * sizeof(GLfloat),          // stride
                (void*) 0   // first element pos
             );

    glVertexAttribPointer   // + color
            (
                1,          // attrib index
                3,          // number of elements
                GL_FLOAT,
                GL_FALSE,   // normalize fixed point
                5 * sizeof(GLfloat),          // stride
                (void*) (2*sizeof(GLfloat))   // first element pos
            );

    bool continueExec = true;
    unsigned ticksChangeColor = 0;
    unsigned frameColor = 0;
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

        // draw stuff
        glClear(GL_COLOR_BUFFER_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glUseProgram(shaderProg);

        glDrawElements(GL_LINE_STRIP, sizeof(inds)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        int ticksDiff = newTicks - ticks;
        ticks = newTicks;

        ticksChangeColor += ticksDiff;
        if(ticksChangeColor > 1000)
        {
            frameColor++;
            GLfloat* vv;
            switch(frameColor)
            {
                case 0: vv = &verts[0]; break;
                case 1: vv = &verts1[0]; break;
                case 2:
                    vv = &verts2[0];
                    frameColor = -1;
                break;
            }
            ticksChangeColor = 0;

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), vv);
        }

        int waitTicks = static_cast<int>(TICKS_PER_FRAME) - (SDL_GetTicks() - newTicks) ;

        if(waitTicks > 0) SDL_Delay(static_cast<unsigned>(waitTicks));

    }

}
