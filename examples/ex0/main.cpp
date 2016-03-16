#include <iostream>
#include <glad/glad.h>
#include <SDL.h>

using namespace std;

const unsigned FRAMES_PER_SEC = 60;
const unsigned TICKS_PER_SEC = 1000;
const unsigned TICKS_PER_FRAME = TICKS_PER_SEC / FRAMES_PER_SEC;

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window =
            SDL_CreateWindow(
                "example 0",
                100, 100,
                800, 600,
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

    glClearColor(0, 0, 0, 0);

    // buffers
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    GLfloat verts[] = {0, 0};
    GLuint inds[] = {0};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

    const char* vertShadSrc =
            "#version 120\n"
            "void main(){"
            "gl_Position = vec4(0,0,0,0);"
            "}";
    const char* fragShadSrc =
            "#version 120\n"
            "void main(){"
            "gl_FragColor = vec4(1, 1, 1, 1);"
            "}";

    // compile shaders
    GLuint vertShad = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShad = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShad, 1, &vertShadSrc, 0);
    glShaderSource(fragShad, 1, &fragShadSrc, 0);

    glCompileShader(vertShad);
    glCompileShader(fragShad);

    GLint compileOk;
    char errorStr[1024];

    glGetShaderiv(vertShad, GL_COMPILE_STATUS, &compileOk);
    if(!compileOk)
    {
        glGetShaderInfoLog(vertShad, sizeof(errorStr), 0, errorStr);
        cout << errorStr;
        exit(1);
    }

    glGetShaderiv(fragShad, GL_COMPILE_STATUS, &compileOk);
    if(!compileOk)
    {
        glGetShaderInfoLog(fragShad, sizeof(errorStr), 0, errorStr);
        cout << errorStr << endl;
        exit(1);
    }

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertShad);
    glAttachShader(shaderProg, fragShad);

    glLinkProgram(shaderProg);
    GLint linkOk;
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkOk);
    if(!linkOk)
    {
        glGetProgramInfoLog(shaderProg, sizeof(errorStr), 0, errorStr);
        cout << errorStr << endl;
        exit(1);
    }

    glValidateProgram(shaderProg);

    glUseProgram(shaderProg);

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

        glDrawElements(GL_POINTS, sizeof(inds)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        ticks = static_cast<int>(SDL_GetTicks()) - ticks;
        int waitTicks = static_cast<int>(TICKS_PER_FRAME) - ticks;

        if(waitTicks > 0) SDL_Delay(static_cast<unsigned>(waitTicks));

    }

}

