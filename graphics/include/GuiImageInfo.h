#ifndef GENG_GUI_IMAGE_INFO_H
#define GENG_GUI_IMAGE_INFO_H

class GuiImageInfo
{
public:
    GuiImageInfo(int guiOrientation, int offsetX, int offsetY, const char * guiImageFileName);
	~GuiImageInfo();
    int getOffSetX() { return offsetX; }
    int getOffSetY() { return offsetY; }
    int getOrientation() { return orientation; }
    const char *getImageFileName() { return imageFileName; }
private:
	int orientation; 
	int offsetX; 
	int offsetY;
   	const char *imageFileName;
};

#endif