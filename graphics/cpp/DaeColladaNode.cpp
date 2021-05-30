#include "DaeColladaNode.h"

DaeColladaNode::DaeColladaNode(XMLNode *node) 
: libraryEffects(), libraryGeometries(), libraryImages(), libraryMaterials(), libraryVisualScenes()
{
    // Process the "library_effects" element.
    int numLibEffectsNodes = node->nChildNode("library_effects");
    if (numLibEffectsNodes == 1)
    {
        XMLNode xmlLibEffects = node->getChildNode("library_effects");
        libraryEffects = new DaeLibraryEffectsNode(&xmlLibEffects);
    }
    else
    {
        //printf("Expected 1 library_effects element, got %d\n", numLibEffectsNodes);
    }
    
    // Process the "library_geometries" element.
    int numLibGeometryNodes = node->nChildNode("library_geometries");
    if (numLibGeometryNodes == 1)
    {
        XMLNode xmlLibGeometries = node->getChildNode("library_geometries");
        libraryGeometries = new DaeLibraryGeometriesNode(&xmlLibGeometries);
    }
    else
    {
        printf("Expected 1 library_geometries element, got %d\n", numLibGeometryNodes);
        return;
    }
    
    // Process the "library_images" element.
    int numLibImageNodes = node->nChildNode("library_images");
    if (numLibImageNodes == 1)
    {
        XMLNode xmlLibImages = node->getChildNode("library_images");
        libraryImages = new DaeLibraryImagesNode(&xmlLibImages);
    }
    else
    {
        //printf("Found %d library_images nodes\n", numLibImageNodes);
    }
    
    // Process the "library_materials" element.
    int numLibMaterialNodes = node->nChildNode("library_materials");
    if (numLibMaterialNodes == 1)
    {
        XMLNode xmlLibMaterials = node->getChildNode("library_materials");
        libraryMaterials = new DaeLibraryMaterialsNode(&xmlLibMaterials);
    }
    else
    {
        //printf("Found %d library_materials nodes\n", numLibMaterialNodes);
    }
    
    // Process the "library_visual_scenes" element.
    int numLibVisualScenes = node->nChildNode("library_visual_scenes");
    if (numLibVisualScenes == 1)
    {
        XMLNode xmlLibVisualScenes = node->getChildNode("library_visual_scenes");
        libraryVisualScenes = new DaeLibraryVisualScenesNode(&xmlLibVisualScenes);
    }
    else
    {
        //printf("Expected 1 library_visual_scenes element, got %d\n", numLibVisualScenes);
        return;
    }
}

DaeColladaNode::~DaeColladaNode()
{
    if (libraryGeometries)
    {
        delete libraryGeometries;
    }
}