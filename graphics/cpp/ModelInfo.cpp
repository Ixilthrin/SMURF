#include "ModelInfo.h"

ModelInfo::ModelInfo()
: hasTexture(0), hasColorValues(0), hasSecondTexture(0),
  hasThirdTexture(0), hasNormals(0),
  animation(0), normals(0), boneWeights(0), boneIndices(0)
{
}

ModelInfo::~ModelInfo()
{
}

