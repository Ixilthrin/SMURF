#ifndef GENG_GUI_VIDEO_H
#define GENG_GUI_VIDEO_H

#define GL_GLEXT_PROTOTYPES 1

#include <string>
#include "SDL_thread.h"
#include "GuiOverlay.h"

#include "SDL_types.h"
extern "C" {
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
}

// For saving audio packets so that our audio_callback has something to get audio data from.
typedef struct PacketQueue 
{
  AVPacketList *first_pkt, *last_pkt;
  int nb_packets;
  int size;                     //a byte size that we get from packet->siz
  SDL_mutex *mutex;
  SDL_cond *cond;
} PacketQueue;

class GuiVideo : public GuiOverlay
{
public:
    GuiVideo(
		int guiOrientation, 
		int offsetX, 
		int offsetY);
	virtual ~GuiVideo();
	void load(const std::string & imageFileName);
    void start(bool isLooping);
    void pause() { paused = true; }
    void resume() { paused = false; }
    void unload();
    void decode();
    virtual void update();
	virtual void draw();
	virtual bool pick(int x, int y) { return false; }
	void processAudio(Uint8 *stream, int len);

private:
	//function to put packets in queue
	int packet_queue_put(AVPacket *pkt);
	//function to get packets from queue
	int packet_queue_get(AVPacket *pkt, int block);
	int decodeAudioFrame(uint8_t *audio_buf, int buf_size);

	AVPacket pkt;
    uint8_t *audio_pkt_data;
    int audio_pkt_size;

    uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
    unsigned int audio_buf_size;
    unsigned int audio_buf_index;

    int guiOrientation; 
    int offsetX; 
    int offsetY;;
    int textureSize;
    AVFormatContext * pFormatCtx;
    AVCodecContext * pCodecCtx;
    AVCodecContext *aCodecCtx;
    int videoStream;
	int audioStream;
	PacketQueue audioq;
    int height;
    int width;
    char * buf[4];
    bool looping;
    bool quit;
    bool paused;
    SDL_Thread * decodeThread;
    unsigned long head;
    unsigned long tail;
    Uint32 deadlineForNextFrame;
};

#endif
