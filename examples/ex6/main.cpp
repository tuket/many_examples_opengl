#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <GL/glew.h>
#include <stb_image.h>
#include <utils.hpp>

using namespace std;

const unsigned FRAMES_PER_SEC = 60;
const unsigned TICKS_PER_SEC = 1000;
const unsigned TICKS_PER_FRAME = TICKS_PER_SEC / FRAMES_PER_SEC;

vector<string> attribNames;
vector<string> textures;

void initNames()
{
    attribNames.push_back("pos");
    attribNames.push_back("texCoord0");

    textures.push_back("tex0");
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
                "example 6",
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
                        "examples/ex6/shaders/vert.glsl",
                        "examples/ex6/shaders/frag.glsl"
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
    GLuint texture;
    glGenTextures(1, &texture);
    GLuint texUnif = glGetUniformLocation(shadProgram, "texture0");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
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
    glUniform1i(texUnifm, 0);

}
