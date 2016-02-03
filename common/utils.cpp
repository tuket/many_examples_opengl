#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

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

    fs.open(vertShadFileName, fstream::in);
    ss << fs;
    src = ss.str();
    GLuint vertShad = glCreateShader(GL_VERTEX_SHADER);
    pSrc = src.c_str();
    glShaderSource(vertShad, 1, &pSrc, 0);
    glCompileShader(vertShad);
    glGetShaderiv(vertShad, GL_COMPILE_STATUS, &compileOkVert);
    if(!compileOkVert)
    {
        glGetShaderInfoLog(vertShad, sizeof(errorStr), 0, errorStr);
        cout << errorStr << endl;
    }

    fs.open(fragShadFileName, fstream::in);
    ss << fs;
    src = ss.str();
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

    program = glCreateProgram();
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
