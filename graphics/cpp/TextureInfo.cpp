#include "TextureInfo.h"

TextureInfo::TextureInfo(bool dynamic, int pixFormat) 
: isDynamic(dynamic), pixelFormat(pixFormat), generateMipmap(0)
{
}

TextureInfo::~TextureInfo()
{
}

