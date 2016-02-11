#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

bool fileToString(string& str, const string& fileName)
{
    fstream fs;
    fs.open(fileName, fstream::in);
    if(!fs.is_open()) return false;
    fs.seekg(0, fs.end);
    int length = fs.tellg();
    fs.seekg(0, fs.beg);
    if(length+1 >= str.size()) str.resize(length+1);
    fs.read(&str[0], length);
    str[length] = 0;
    return true;
}

bool makeShaderProgram
(
    GLuint &program,
    const string& vertShadFileName,
    const string& fragShadFileName
)
{

    static char errorStr[1024];

    fstream fs;
    string src;
    const char* pSrc;
    stringstream ss;
    int length;
    GLint compileOkVert, compileOkFrag;

    if(!fileToString(src, vertShadFileName))
    {
        cout << "unable to open: "
             << vertShadFileName << endl;
    }
    pSrc = src.c_str();
    GLuint vertShad = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShad, 1, &pSrc, 0);
    glCompileShader(vertShad);
    glGetShaderiv(vertShad, GL_COMPILE_STATUS, &compileOkVert);
    if(!compileOkVert)
    {
        glGetShaderInfoLog(vertShad, sizeof(errorStr), 0, errorStr);
        cout << errorStr << endl;
    }

    if(!fileToString((src), fragShadFileName))
    {
        cout << "unable to open: "
             << fragShadFileName << endl;
    }
    pSrc = src.c_str();
    GLuint fragShad = glCreateShader(GL_FRAGMENT_SHADER);
    pSrc = src.c_str();
    glShaderSource(fragShad, 1, &pSrc, 0);
    glCompileShader(fragShad);
    glGetShaderiv(fragShad, GL_COMPILE_STATUS, &compileOkFrag);
    if(!compileOkFrag)
    {
        glGetShaderInfoLog(fragShad, sizeof(errorStr), 0, errorStr);
        cout << errorStr << endl;
    }

    if(!compileOkVert || !compileOkFrag) return false;

    glAttachShader(program, vertShad);
    glAttachShader(program, fragShad);
    glLinkProgram(program);
    GLint linkOk;
    glGetProgramiv(program, GL_LINK_STATUS, &linkOk);
    if(!linkOk)
    {
        glGetProgramInfoLog(program, sizeof(errorStr), 0, errorStr);
        cout << errorStr << endl;
        return false;
    }

    glValidateProgram(program);
    GLint validateOk;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validateOk);
    if(!validateOk)
    {
        glGetProgramInfoLog(program, sizeof(errorStr), 0, errorStr);
        cout << errorStr << endl;
        return false;
    }

    return true;

}

void invertY(GLubyte* data, int w, int h)
{
    unsigned* d = (unsigned*)data;
    for(int y=0; y<h/2; y++)
    {
        for(int x=0; x<w; x++)
        {
            swap( data[x + y*w], data[x + (h-1-y)*w] );
        }
    }
}

GLubyte* loadImage(const string& fileName, int &w, int &h, int& comp)
{
    GLubyte* data = stbi_load(fileName.c_str(), &w, &h, &comp, 4);
    invertY(data, w, h);
    return data;
}

void freeImage(GLubyte* imgData)
{
    stbi_image_free(imgData);
}
