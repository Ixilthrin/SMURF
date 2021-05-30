#ifndef GENG_TEXTURE_INFO_H
#define GENG_TEXTURE_INFO_H

class TextureInfo
{
public:
    TextureInfo(bool dynamic, int pixFormat);
	~TextureInfo();
    int getPixelFormat() { return pixelFormat; }
    void setGenerateMipmap(bool value) { generateMipmap = value; }
    bool getGenerateMipmap() { return generateMipmap; }
    void setPixelData(void *data) { pixelData = data; }
    void *getPixelData() { return pixelData; }
    void setWidth(int w) { width = w; }
    int getWidth() { return width; }
    void setHeight(int h) { height = h; }
    int getHeight() { return height; }
	bool getIsDynamic() { return isDynamic; }
    
    enum {
		RGB16_FORMAT,
        RGB24_FORMAT,
        ARGB32_FORMAT
    };
private:
	bool isDynamic;  // Indicate if texture will be modified, as with video.
    int pixelFormat;
    bool generateMipmap;
    void *pixelData;
    int width;
    int height;
};

#endif