#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <GL/glew.h>
#include "utils.hpp"

using namespace std;

const unsigned FRAMES_PER_SEC = 60;
const unsigned TICKS_PER_SEC = 1000;
const unsigned TICKS_PER_FRAME = TICKS_PER_FRAME / FRAMES_PER_SEC;

vector<string> attribNames;

void initAttribNames()
{
    attribNames.push_back("pos");
}

int main()
{

    initAttribNames();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    SDL_Window *window =
            SDL_CreateWindow
            (
                "example 5",
                100, 100,
                800, 600,
                SDL_WINDOW_OPENGL
            );

    //SDL_GLContext context =
            SDL_GL_CreateContext(window);

    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        cout << "Error: glewInit(): "
             << glewGetErrorString(glewError)
             << endl;
        exit(1);
    }
    cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;

    glClearColor(0.1, 0.1, 0.1, 1);

    // buffers
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    GLfloat verts[]=
    {
        0, 0.5,
        0.5, -0.5,
        -0.5, -0.5
    };

    GLuint inds[]=
    {
        0, 1, 2
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

    // shaders

    GLuint shadProgram = glCreateProgram();

    glBindAttribLocation(shadProgram, 0, attribNames[0].c_str());

    bool programOk = makeShaderProgram(shadProgram, "examples/ex5/shaders/vert.glsl", "examples/ex5/shaders/frag.glsl");

    if(!programOk)
    {
        cout << "error in the shader program" << endl;
        exit(1);
    }

    glEnableVertexAttribArray(0);

    glVertexAttribPointer   // + pos
            (
                0,          // attrib index
                2,          // number of elements
                GL_FLOAT,
                GL_FALSE,   // normalize fixed point
                2 * sizeof(GLfloat),          // stride
                (void*) 0   // first element pos
             );

    // uniforms
    GLuint globColorUnif = glGetUniformLocation(shadProgram, "u_globColor");
    glUniform3f(globColorUnif, 1, 1, 1);

    bool continueExec = true;
    int ticks = SDL_GetTicks();
    bool arrows[3] = {0};
    while(continueExec)
    {
        int newTicks = static_cast<int>(SDL_GetTicks());

        // handle events
        SDL_Event event;
        SDL_PollEvent(&event);
        bool keyPressed = false;
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
                case SDLK_LEFT:
                    arrows[0] = true;
                break;
                case SDLK_DOWN:
                    arrows[1] = true;
                break;
                case SDLK_RIGHT:
                    arrows[2] = true;
                break;
            }
            keyPressed = true;
        }
        else if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    arrows[0] = false;
                break;
                case SDLK_DOWN:
                    arrows[1] = false;
                break;
                case SDLK_RIGHT:
                    arrows[2] = false;
                break;
            }
            keyPressed = true;
        }

        if(keyPressed)
        {
            glUniform3f
                (
                    globColorUnif,
                    arrows[0] ? 1 : 0,
                    arrows[1] ? 1 : 0,
                    arrows[2] ? 1 : 0
                );
        }

        // draw stuff
        glClear(GL_COLOR_BUFFER_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glUseProgram(shadProgram);

        glDrawElements(GL_TRIANGLES, sizeof(inds)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        int ticksDiff = newTicks - ticks;
        ticks = newTicks;

        int waitTicks = static_cast<int>(TICKS_PER_FRAME) - (SDL_GetTicks() - newTicks) ;

        if(waitTicks > 0) SDL_Delay(static_cast<unsigned>(waitTicks));

    }

}
