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
#include <iomanip>

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
    const static string projMat;
    const static string viewMat;
    const static string modelMat;
    const static string modelViewMat;
    const static string ambientLight;
    const static string dirLight;
};

const string UniformNames::transfMat = "transfMat";
const string UniformNames::projMat = "projMat";
const string UniformNames::viewMat = "viewMat";
const string UniformNames::modelMat = "modelMat";
const string UniformNames::modelViewMat = "modelViewMat";
const string UniformNames::ambientLight = "ambientLight";
const string UniformNames::dirLight = "dirLight";

void initNames()
{
    attribNames.push_back("pos");
    attribNames.push_back("texCoord0");
    attribNames.push_back("normal");
    attribNames.push_back("tangent");
    attribNames.push_back("bitangent");

    textures.push_back("tex0");
    textures.push_back("texNormal");
    textures.push_back("texSpecular");
}

struct TexSlots{
    enum
    {
        DIFFUSE = 0,
        NORMAL,
        SPECULAR,
    };
};

GLuint bindTexture(GLuint shadProg, GLuint texSlot, const char* texPath);

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
                "mesh/wood_box.obj",
                aiProcess_CalcTangentSpace      |
                aiProcess_Triangulate           |
                aiProcess_JoinIdenticalVertices |
                aiProcess_SortByPType           |
                aiProcess_GenNormals
            );

    if(!scene)
    {
        cout << "error loading mesh" << endl;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    const aiString diffuseTexName("img/brick_texture.jpg");
    const aiString normalTexName("img/brick_normal.jpg");
    const aiString specularTexName("img/brick_specular.jpg");

    // buffers
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    const unsigned floatsPerVert = 14;
    const unsigned vertsSize = floatsPerVert * mesh->mNumVertices;
    GLfloat* verts = new GLfloat[vertsSize];
    for(unsigned i=0; i<mesh->mNumVertices; i++)
    {
        verts[floatsPerVert*i + 0] = mesh->mVertices[i].x;
        verts[floatsPerVert*i + 1] = mesh->mVertices[i].y;
        verts[floatsPerVert*i + 2] = mesh->mVertices[i].z;
        verts[floatsPerVert*i + 3] = mesh->mTextureCoords[0][i].x;
        verts[floatsPerVert*i + 4] = mesh->mTextureCoords[0][i].y;
        verts[floatsPerVert*i + 5] = mesh->mNormals[i].x;
        verts[floatsPerVert*i + 6] = mesh->mNormals[i].y;
        verts[floatsPerVert*i + 7] = mesh->mNormals[i].z;
        verts[floatsPerVert*i + 8] = mesh->mTangents[i].x;
        verts[floatsPerVert*i + 9] = mesh->mTangents[i].y;
        verts[floatsPerVert*i + 10] = mesh->mTangents[i].z;
        verts[floatsPerVert*i + 11] = mesh->mBitangents[i].x;
        verts[floatsPerVert*i + 12] = mesh->mBitangents[i].y;
        verts[floatsPerVert*i + 13] = mesh->mBitangents[i].z;
    }

    const unsigned indsSize = 3 * mesh->mNumFaces;
    GLuint* inds = new GLuint[indsSize];
    for(unsigned i=0; i<mesh->mNumFaces; i++)
    {
        inds[3*i + 0] = mesh->mFaces[i].mIndices[0];
        inds[3*i + 1] = mesh->mFaces[i].mIndices[1];
        inds[3*i + 2] = mesh->mFaces[i].mIndices[2];
    }

    // debug
    for(int face=0; face<mesh->mNumFaces; face++)
    {
        cout << "face " << face << ": " << endl;
        cout << setprecision(0) << fixed;

        cout << "\tv0: " << mesh->mFaces[face].mIndices[0] << endl;
        cout << "\t\tpos:"
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[0] + 0] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[0] + 1] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[0] + 2] << endl;
        cout << "\t\tnorm:"
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[0] + 5] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[0] + 6] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[0] + 7] << endl;

        cout << "\tv1: " << mesh->mFaces[face].mIndices[1] << endl;
        cout << "\t\tpos:"
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[1] + 0] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[1] + 1] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[1] + 2] << endl;
        cout << "\t\tnorm:"
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[1] + 5] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[1] + 6] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[1] + 7] << endl;

        cout << "\tv2: " << mesh->mFaces[face].mIndices[2] << endl;
        cout << "\t\tpos:"
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[2] + 0] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[2] + 1] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[2] + 2] << endl;
        cout << "\t\tnorm:"
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[2] + 5] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[2] + 6] << " "
            << verts[floatsPerVert * mesh->mFaces[face].mIndices[2] + 7] << endl;
    }

    glBufferData(GL_ARRAY_BUFFER, vertsSize * sizeof(GLfloat), verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsSize * sizeof(GLuint), inds, GL_STATIC_DRAW);

    // shaders

    GLuint shadProgram = glCreateProgram();

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

    for(unsigned i=0; i<attribNames.size(); i++)
        glBindAttribLocation(shadProgram, i, attribNames[i].c_str());

    glVertexAttribPointer               // + pos
            (
                0,                      // attrib index
                3,                      // number of elements
                GL_FLOAT,
                GL_FALSE,               // normalize fixed point
                floatsPerVert * sizeof(GLfloat),    // stride
                (void*) 0               // first element pos
            );

    glVertexAttribPointer                       // + tex coord
            (
                1,                              // attrib index
                2,                              // number of elements
                GL_FLOAT,
                GL_FALSE,                       // normalize fixed point
                floatsPerVert * sizeof(GL_FLOAT),           // stride
                (void*) (3*sizeof(GLfloat))     // first element pos
            );

    glVertexAttribPointer                       // + normal
            (
                2,                              // attrib index
                3,                              // number of elements
                GL_FLOAT,
                GL_FALSE,                       // normalize fixed point
                floatsPerVert * sizeof(GL_FLOAT),           // stride
                (void*) (5*sizeof(GLfloat))     // first element pos
            );

    glVertexAttribPointer                       // + tangent
            (
                3,                              // attrib index
                3,                              // number of elements
                GL_FLOAT,
                GL_FALSE,                       // normalize fixed point
                floatsPerVert * sizeof(GL_FLOAT),           // stride
                (void*) (8*sizeof(GLfloat))     // first element pos
            );

    glVertexAttribPointer                       // + bitengent
            (
                4,                              // attrib index
                3,                              // number of elements
                GL_FLOAT,
                GL_FALSE,                       // normalize fixed point
                floatsPerVert * sizeof(GL_FLOAT),           // stride
                (void*) (11*sizeof(GLfloat))     // first element pos
            );

    // uniforms & textures
    glUseProgram(shadProgram);      // it's important to use the shader before
    bindTexture(shadProgram, TexSlots::DIFFUSE, diffuseTexName.C_Str());
    bindTexture(shadProgram, TexSlots::NORMAL, normalTexName.C_Str());
    bindTexture(shadProgram, TexSlots::SPECULAR, specularTexName.C_Str());

    GLuint transfMatUnif = glGetUniformLocation(shadProgram, UniformNames::transfMat.c_str());
    Camera cam;
    cam.setPosition(glm::vec3(0, 0, 3));
    glm::mat4 mat = cam.getTransfMat();
    glUniformMatrix4fv(transfMatUnif, 1, GL_FALSE, glm::value_ptr(mat));

    GLuint projMatUnif = glGetUniformLocation(shadProgram, UniformNames::projMat.c_str());
    mat = cam.getProjMat();
    glUniformMatrix4fv(projMatUnif, 1, GL_FALSE, glm::value_ptr(mat));

    GLuint viewMatUnif = glGetUniformLocation(shadProgram, UniformNames::viewMat.c_str());
    mat = cam.getViewMat();
    glUniformMatrix4fv(viewMatUnif, 1, GL_FALSE, glm::value_ptr(mat));

    GLuint modelMatUnif = glGetUniformLocation(shadProgram, UniformNames::modelMat.c_str());
    mat = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatUnif, 1, GL_FALSE, glm::value_ptr(mat));

    GLuint modelViewMatUnif = glGetUniformLocation(shadProgram, UniformNames::modelViewMat.c_str());
    mat = cam.getViewMat();
    glUniformMatrix4fv(modelViewMatUnif, 1, GL_FALSE, glm::value_ptr(mat));

    GLuint ambientLightUnif = glGetUniformLocation(shadProgram, UniformNames::ambientLight.c_str());
    const GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f};
    glUniform3fv(ambientLightUnif, 1, ambientLight);

    GLuint dirLightColorUnif = glGetUniformLocation(shadProgram, (UniformNames::dirLight + ".color").c_str());
    GLuint dirLightDirUnif = glGetUniformLocation(shadProgram, (UniformNames::dirLight + ".dir").c_str());
    const GLfloat dirLightColor[] = {0.9f, 0.9f, 0.9f};
    const GLfloat dirLightDir[] = {1.0f, 0.0f, -1.0f};
    glm::vec3 dirLightDirVec = glm::vec3(dirLightDir[0], dirLightDir[1], dirLightDir[2]);
    dirLightDirVec = glm::normalize(dirLightDirVec);
    glUniform3fv(dirLightColorUnif, 1, dirLightColor);
    glUniform3fv(dirLightDirUnif, 1, glm::value_ptr(dirLightDirVec));

    int prevMouseX, prevMouseY;
    bool prevMousePressed = false;

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
        bool latRightPressed = keyState[SDL_SCANCODE_E];

        // camera movement
        const float CAM_SPEED = 0.05;
        glm::vec3 disp;
        float h = 0, v = 0;
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
            h = degToRad(-0.5);
        }
        else if(rightPressed)
        {
            h = degToRad(+0.5);
        }
        if(latLeftPressed)
        {
            disp += glm::vec3(-1, 0, 0);
        }
        else if(latRightPressed)
        {
            disp += glm::vec3(1, 0, 0);
        }

        // mouse
        int mouseX, mouseY;
        Uint32 mouseMask = SDL_GetMouseState(&mouseX, &mouseY);
        if(mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            const float MOUSE_SENSITIVITY = 0.005f;
            if(prevMousePressed == false)
            {
                prevMousePressed = true;
            }
            else
            {
                int difX = mouseX - prevMouseX;
                int difY = mouseY - prevMouseY;
                h += MOUSE_SENSITIVITY * difX;
                v -= MOUSE_SENSITIVITY * difY;
            }
            prevMouseX = mouseX;
            prevMouseY = mouseY;
        }
        else
        {
            prevMousePressed = false;
        }

        disp = disp / disp.length();
        disp *= CAM_SPEED;
        float heading = cam.getHeading();
        heading -= h;
        float pitch = cam.getPitch();
        pitch += v;
        cam.translateLocal(disp);
        cam.setPitch(pitch);
        cam.setHeading(heading);
        mat = cam.getTransfMat();
        glUniformMatrix4fv(transfMatUnif, 1, GL_FALSE, glm::value_ptr(mat));

        // draw stuff
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glUseProgram(shadProgram);

        glDrawElements(GL_TRIANGLES, indsSize, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);

        int ticksDiff = newTicks-ticks;
        ticks = newTicks;

        int waitTicks = static_cast<int>(TICKS_PER_FRAME) - (SDL_GetTicks() - newTicks);

        if(waitTicks > 0) SDL_Delay(static_cast<unsigned>(waitTicks));

    }

}


GLuint bindTexture(GLuint shadProg, GLuint texSlot, const char* texPath)
{

    GLuint tex;
    glGenTextures(1, &tex);
    GLuint unif = glGetUniformLocation(shadProg, textures[texSlot].c_str());

    glActiveTexture(GL_TEXTURE0 + texSlot);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    int w, h, comp;
    GLubyte* imgData = loadImage(texPath, w, h, comp);
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
    glUniform1i(unif, texSlot);

}
