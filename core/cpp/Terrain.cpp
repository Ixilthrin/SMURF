#include "Terrain.h"
#include "utXMLParser.h"
#include "util.h"
#include "xml_util.h"
#include "PlaneTerrain.h"
//#include "Physics.h"
#include "btBulletDynamicsCommon.h"

Terrain * Terrain::load(const std::string & terrainXml, int maxProxies)
{
    Terrain * terrain = 0;
    assertFileExists(terrainXml);
    XMLNode rootNode = XMLNode::parseFile(terrainXml.c_str());
    if (rootNode.isEmpty())
    {
        fatalError("Bad XML file: " + terrainXml);
    }
    std::string rootNodeName(rootNode.getName());
    if (rootNodeName != "terrain")
    {
        fatalError(std::string("*") + rootNode.getName() + "*");
        fatalError("Root element of terrain file not \"terrain\": " + terrainXml);
    }

    // Determine extent of terrain.

    XMLNode extentNode = rootNode.getChildNode("extent");
    if (extentNode.isEmpty())
    {
        fatalError("Extent element missing from " + terrainXml);
    }

    // Determine min aabb.

    XMLNode minXNode = extentNode.getChildNode("minX");
    if (minXNode.isEmpty())
    {
        fatalError("minX element missing from " + terrainXml);
    }
    float minX = stringToFloat(minXNode.getText());

    XMLNode minYNode = extentNode.getChildNode("minY");
    if (minYNode.isEmpty())
    {
        fatalError("minY element missing from " + terrainXml);
    }
    float minY = stringToFloat(minYNode.getText());

    XMLNode minZNode = extentNode.getChildNode("minZ");
    if (minZNode.isEmpty())
    {
        fatalError("minZ element missing from " + terrainXml);
    }
    float minZ = stringToFloat(minZNode.getText());

    btVector3 aabbMin(minX, minY, minZ);

    // Determine max aabb.

    XMLNode maxXNode = extentNode.getChildNode("maxX");
    if (maxXNode.isEmpty())
    {
        fatalError("maxX element missing from " + terrainXml);
    }
    float maxX = stringToFloat(maxXNode.getText());

    XMLNode maxYNode = extentNode.getChildNode("maxY");
    if (maxYNode.isEmpty())
    {
        fatalError("maxY element missing from " + terrainXml);
    }
    float maxY = stringToFloat(maxYNode.getText());

    XMLNode maxZNode = extentNode.getChildNode("maxZ");
    if (maxZNode.isEmpty())
    {
        fatalError("maxZ element missing from " + terrainXml);
    }
    float maxZ = stringToFloat(maxZNode.getText());

    btVector3 aabbMax(maxX, maxY, maxZ);

    // Process the ground type.

    XMLNode groundNode = rootNode.getChildNode("ground");
    if (groundNode.isEmpty())
    {
        fatalError("Ground element missing from " + terrainXml);
    }
    XMLNode groundTypeNode = groundNode.getChildNode("type");
    if (groundTypeNode.isEmpty())
    {
        fatalError("Ground type element missing from " + terrainXml);
    }
    std::string groundType = groundTypeNode.getText();
    if (groundType == "plane")
    {
        terrain = new PlaneTerrain(rootNode, maxProxies, aabbMin, aabbMax);
    }
    else
    {
        fatalError("Unknown ground type in " + terrainXml);
    }
    return terrain;
}

Terrain::Terrain(int maxProxies, const btVector3 & aabbMin, const btVector3 & aabbMax)
{
//    Physics * physics = Physics::getInstance();
//    physics->createDynamicsWorld(maxProxies, aabbMin, aabbMax);
}

Terrain::~Terrain()
{
}
