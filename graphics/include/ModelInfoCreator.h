#ifndef GENG_MODEL_INFO_CREATOR_H
#define GENG_MODEL_INFO_CREATOR_H

#include "DaeColladaNode.h"
#include "ModelInfo.h"
#include "XFile.h"

class ModelInfoCreator
{
public:
    ModelInfoCreator(DaeColladaNode *theColladaNode);
    ModelInfoCreator(XFile *theXFile);
    ModelInfo *getModelInfo();
private:
    ModelInfo *modelInfo;
    DaeColladaNode *colladaNode;
    XFile *xFile;
    ModelInfo *createFromColladaNode();
    ModelInfo *createFromXFile();
};

#endif