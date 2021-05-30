#ifndef GENG_DAE_FILE
#define GENG_DAE_FILE

//#include "core.h"
#include "DaeColladaNode.h"
#include "Model.h"
#include "utXMLParser.h"
#include <string>
#include <vector>
#include "ModelInfo.h"

// Encapsulates "relevant" information contained in a Collada file.
class DaeFile
{
public:
	DaeFile();
	~DaeFile();
	void load(std::string fileName);
    static std::vector<std::string> tokenizeString(char delimiter, std::string theString);
    DaeColladaNode *getColladaNode() { return colladaNode; }

private:
    DaeColladaNode *colladaNode;
    std::string theFileName;
};

#endif
