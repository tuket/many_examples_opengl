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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
    const static string ambientLight;
    const static string dirLight;
};

const string UniformNames::transfMat = "transfMat";
const string UniformNames::ambientLight = "ambientLight";
const string UniformNames::dirLight = "dirLight";

void initNames()
{
    attribNames.push_back("pos");
    attribNames.push_back("texCoord0");

    textures.push_back("tex0");
    textures.push_back("texNormal");
}

int main(int argc, char** argv)
{

    initNames();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window =
            SDL_CreateWindow
            (
                "example 11",
                100, 100,
                800, 600,
                SDL_WINDOW_OPENGL
            );

    //SDL_GLContext context =
    SDL_GL_CreateContext(window);

    if(!gladLoadGL())
    {
        cout << "glad error!" << endl;
        return -1;
    }
    cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << endl;
    if (GLVersion.major < 2) {
        cout << "Your system doesn't support OpenGL >= 2!\n" << endl;
        return -1;
    }
    cout << "OpenGL " << glGetString(GL_VERSION)
         << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    glClearColor(0.1, 0.1, 0.1, 1);
    glEnable(GL_DEPTH_TEST);

    // loading the mesh
    Assimp::Importer importer;
    const aiScene* scene =
            importer.ReadFile
            (
                "mesh/textured_box",
                aiProcess_CalcTangentSpace      |
                aiProcess_Triangulate           |
                aiProcess_JoinIdenticalVertices |
                aiProcess_SortByPType
            );

    if(!scene)
    {
        cout << "error loading mesh" << endl;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    const aiString diffuseTexName("img/brick_texture.jpg");
    const aiString normalTexName("img/brick_normal.jpg");
    const siString specularTexName("img/brick_specular.jpg");

    // buffers
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    const unsigned vertsSize = 5 * mesh->mNumVertices;
    GLfloat* verts = new GLfloat[vertsSize];
    for(unsigned i=0; i<mesh->mNumVertices; i++)
    {
        verts[5*i + 0] = mesh->mVertices[i].x;
        verts[5*i + 1] = mesh->mVertices[i].y;
        verts[5*i + 2] = mesh->mVertices[i].z;
        verts[5*i + 3] = mesh->mTextureCoords[0][i].x;
        verts[5*i + 4] = mesh->mTextureCoords[0][i].y;
    }

    const unsigned indsSize = 3 * mesh->mNumFaces;
    GLuint* inds = new GLuint[indsSize];
    for(unsigned i=0; i<mesh->mNumFaces; i++)
    {
        inds[3*i + 0] = mesh->mFaces[i].mIndices[0];
        inds[3*i + 1] = mesh->mFaces[i].mIndices[1];
        inds[3*i + 2] = mesh->mFaces[i].mIndices[2];
    }

    glBufferData(GL_ARRAY_BUFFER, vertsSize * sizeof(GLfloat), verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsSize * sizeof(GLuint), inds, GL_STATIC_DRAW);

    // shaders

    GLuint shadProgram = glCreateProgram();

    glBindAttribLocation(shadProgram, 0, attribNames[0].c_str());
    glBindAttribLocation(shadProgram, 1, attribNames[1].c_str());

    bool programOk = makeShaderProgram
                    (
                        shadProgram,
                        "examples/ex11/shaders/vert.glsl",
                        "examples/ex11/shaders/frag.glsl"
                    );

    if(!programOk)
    {
        cout << "error in the shader program" << endl;
        return -1;
    }

    for(unsigned i=0; i<attribNames.size(); i++)
        glEnableVertexAttribArray(i);

    glVertexAttribPointer               // + pos
            (
                0,                      // attrib index
                3,                      // number of elements
                GL_FLOAT,
                GL_FALSE,               // normalize fixed point
                5 * sizeof(GLfloat),    // stride
                (void*) 0               // first element pos
            );

    glVertexAttribPointer                       // + tex coord
            (
                1,                              // attrib index
                2,                              // number of elements
                GL_FLOAT,
                GL_FALSE,                       // normalize fixed point
                5 * sizeof(GL_FLOAT),           // stride
                (void*) (3*sizeof(GLfloat))     // first element pos
            );

    // uniforms & textures
    glUseProgram(shadProgram);      // it's important to use the shader before
    GLuint diffuseTex, normalTex;
    glGenTextures(1, &diffuseTex);
    glGenTextures(1, &normalTex);
    GLuint diffuseTexUnif = glGetUniformLocation(shadProgram, textures[0].c_str());
    GLuint normalTexUnif = glGetUniformLocation(shadProgram, textures[1].c_Str());

    GLuint textureId = 0;
    glActiveTexture(GL_TEXTURE0 + textureId);
    glBindTexture(GL_TEXTURE_2D, diffuseTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    int w, h, comp;
    GLubyte* imgData = loadImage(diffuseTexName, w, h, comp);
    glTexImage2D
            (
                GL_TEXTURE_2D,
                0,                  // mipmap reduction level
                GL_RGBA,
                w, h,
                0,                  // border: this value must be 0
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                imgData
            );
    freeImage(imgData);
    glUniform1i(diffuseTexUnif, textureId);

    textureId = 1;
    glActiveTexture(GL_TEXTURE0 + textureId);
    glBindTexture(GL_TEXTURE_2D, normalTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    imgData = loadImage(normalTexName, w, h, comp);
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


}
