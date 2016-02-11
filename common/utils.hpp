#include <iostream>
#include <GL/glew.h>

bool makeShaderProgram
(
    GLuint &program,
    const std::string& vertShadFileName,
    const std::string& fragShadFileName
);

GLubyte* loadImage(const std::string& fileName, int &w, int &h, int& comp);

void freeImage(GLubyte* imgData);
