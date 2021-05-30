#include "ShaderReference.h"
#include "util.h"
#include <stdlib.h>

ShaderReference::ShaderReference(int theProgramType) 
: programType(theProgramType)
{
}

char *ShaderReference::getShaderCode(std::string fileName)
{
    assertFileExists(fileName);
    // Allocate memory to hold the source of our shaders.
    FILE *fp;
    int count, pos, shaderSize;
    
    fp = fopen(fileName.c_str(), "r");
    if (!fp)
        return 0;
    
    pos = (int) ftell (fp);
    fseek (fp, 0, SEEK_END);            //move to end
    shaderSize = (int) ftell(fp) - pos;    //calculates file size    
    fseek (fp, 0, SEEK_SET);             //rewind to beginning
    
    if (shaderSize <= 0){
        printf("Shader %s empty\n", fileName.c_str());
        return 0;
    }
    
    char *shaderCode = (char *) malloc(shaderSize + 1);
    
    // Read the source code
    
    count = (int) fread(shaderCode, 1, shaderSize, fp);
    shaderCode[count] = '\0';
    
    if (ferror(fp))
    {
        count = 0;
        // TODO: Report this to the appropriate authorities.
    }
    
    fclose(fp);

    return shaderCode;
}

