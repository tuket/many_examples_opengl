#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <fstream> ///

using namespace std;

int main()
{

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

    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        cout << "Error: glewInit(): "
             << glewGetErrorString(glewError)
             << endl;
        exit(1);
    }
    cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;

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
    GLuint inds[] = {0, 1, 2};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

    fstream fs;
    fs.open("shaders/vert.glsl");
    if(!fs.is_open()) cout << "no" << endl;



}
