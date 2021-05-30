#ifndef GENG_DAE_COLLADA_NODE
#define GENG_DAE_COLLADA_NODE

#include <cstdio>
#include "DaeLibraryEffectsNode.h"
#include "DaeLibraryGeometriesNode.h"
#include "DaeLibraryImagesNode.h"
#include "DaeLibraryMaterialsNode.h"
#include "DaeLibraryVisualScenesNode.h"
#include <string>
#include "utXMLParser.h"


// Represents the "COLLADA" element in a Collada file
class DaeColladaNode
{
public:
    DaeColladaNode(XMLNode *node);
    virtual ~DaeColladaNode();
    DaeLibraryEffectsNode * getLibraryEffects() { return libraryEffects; }
    DaeLibraryGeometriesNode * getLibraryGeometries() { return libraryGeometries; }
    DaeLibraryImagesNode * getLibraryImages() { return libraryImages; }
    DaeLibraryMaterialsNode * getLibraryMaterials() { return libraryMaterials; }
    DaeLibraryVisualScenesNode * getLibraryVisualScenes() { return libraryVisualScenes; }
    void setFileName(std::string theFileName) { fileName = theFileName; }
    std::string getFileName() { return fileName; }
private:
    std::string fileName;
    DaeLibraryEffectsNode * libraryEffects;
    DaeLibraryGeometriesNode * libraryGeometries;
    DaeLibraryImagesNode * libraryImages;
    DaeLibraryMaterialsNode * libraryMaterials;
    DaeLibraryVisualScenesNode * libraryVisualScenes;
};

#endif
