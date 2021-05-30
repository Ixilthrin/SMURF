#include "CgShaderReference.h"
#include "util.h"

std::map<std::string, CGcontext> CgShaderReference::contextMap;

CgShaderReference::CgShaderReference(int theProgramType) 
: ShaderReference(theProgramType)
{
}

void CgShaderReference::checkForCgError(const char *situation, CGcontext context)
{
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(context));
    }
    const char *last = cgGetLastListing(context);
    if (!last)
        last = string;
    fatalError(last);
  }
}

