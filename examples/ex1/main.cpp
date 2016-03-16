#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "utils.hpp"
#include <vector>

using namespace std;

const unsigned FRAMES_PER_SEC = 60;
const unsigned TICKS_PER_SEC = 1000;
const unsigned TICKS_PER_FRAME = TICKS_PER_SEC / FRAMES_PER_SEC;

vector<string> attribNames;

void initAttribNames()
{
    attribNames.push_back("pos");
}

int main(int argc, char** argv)
{

    initAttribNames();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window =
            SDL_CreateWindow(
                "example1",
                100, 100,
                800,600,
                SDL_WINDOW_OPENGL
            );

    // SDL_GLContext context =
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

    GLfloat verts[] =
    {
        0, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };
    GLuint inds[] = {0, 1, 2, 0};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

    bool programOk;
    GLuint shaderProg = glCreateProgram();

    glBindAttribLocation(shaderProg, 0, attribNames[0].c_str());

    programOk =
        makeShaderProgram
                (
                    shaderProg,
                    "examples/ex1/shaders/vert.glsl",
                    "examples/ex1/shaders/frag.glsl"
                );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*)0);

    bool continueExec = true;
    while(continueExec)
    {
        int ticks = static_cast<int>(SDL_GetTicks());

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

        ticks = static_cast<int>(SDL_GetTicks()) - ticks;
        int waitTicks = static_cast<int>(TICKS_PER_FRAME) - ticks;

        if(waitTicks > 0) SDL_Delay(static_cast<unsigned>(waitTicks));

    }


}
