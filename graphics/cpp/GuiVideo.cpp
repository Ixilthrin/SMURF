#include <iostream>
#include "Gui.h"
#include "GuiVideo.h"
#include "util.h"
#include "SDL.h"

#include "GraphicsSystem.h"
#include "Graphics.h"
#include "TextureInfo.h"
#include "TextureReference.h"
#include "VertexBufferInfo.h"
#include "VertexBufferReference.h"
#include "IndexBufferInfo.h"
#include "IndexBufferReference.h"
#include "Vertex1.h"

// TODO: Move these into the GuiVideo class.
static TextureInfo *textureInfo = 0;
static TextureReference *textureReference = 0;
static VertexBufferInfo *vertexInfo = 0;
static VertexBufferReference *vertexReference = 0;
static IndexBufferReference *ibRef = 0;

int callDecode(void * data)
{
    GuiVideo * video = reinterpret_cast<GuiVideo *>(data);
    video->decode();
    return 0;
}

int callRender(void * data)
{
    GuiVideo * video = reinterpret_cast<GuiVideo *>(data);
    return 0;
}

int GuiVideo::decodeAudioFrame(uint8_t *audio_buf, int buf_size) 
{
  int len1, data_size;
  for(;;) {
    while(audio_pkt_size > 0) {
      data_size = buf_size;
      len1 = avcodec_decode_audio2(aCodecCtx, (int16_t *)audio_buf, &data_size,
                                  audio_pkt_data, audio_pkt_size);
      if(len1 < 0) {
        /* if error, skip frame */
        audio_pkt_size = 0;
        break;
      }
      audio_pkt_data += len1;
      audio_pkt_size -= len1;
      if(data_size <= 0) {
        /* No data yet, get more frames */
        continue;
      }
      /* We have data, return it and come back for more later */
      return data_size;
    }
    if(pkt.data)
    //  av_free_packet(&pkt);
    if(quit)
      return -1;

    if(packet_queue_get(&pkt, 1) < 0) {
      return -1;
    }
    audio_pkt_data = pkt.data;
    audio_pkt_size = pkt.size;
  }
} //audio_decode_frame


void audio_callback(void * userdata, Uint8 *stream, int len)
{
	GuiVideo * video = reinterpret_cast<GuiVideo *>(userdata);
	video->processAudio(stream, len);
}

void GuiVideo::processAudio(Uint8 *stream, int len)
{
  int len1, audio_size;

  while(len > 0) {
    if(audio_buf_index >= audio_buf_size) {
      /* We have already sent all our data; get more */
      audio_size = decodeAudioFrame(audio_buf, sizeof(audio_buf));
      if(audio_size < 0) {
        /* If error, output silence */
        audio_buf_size = 1024; // arbitrary?
        memset(audio_buf, 0, audio_buf_size);
      } else {
        audio_buf_size = audio_size;
      }
      audio_buf_index = 0;
    }
    len1 = audio_buf_size - audio_buf_index;
    if(len1 > len)
      len1 = len;
    memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
   len -= len1;
    stream += len1;
    audio_buf_index += len1;
  }
}

GuiVideo::GuiVideo(int guiOrientation, int offsetX, int offsetY) : 
    guiOrientation(guiOrientation), 
    offsetX(offsetX), 
    offsetY(offsetY), 
    width(0), 
    height(0), 
    pFormatCtx(0), 
	pCodecCtx(0),
    aCodecCtx(0), 
    videoStream(-1), 
    audioStream(-1), 
    looping(false),
    quit(false),
    head(0),
    tail(0),
    paused(false),
    audio_pkt_data(0),
    audio_pkt_size(0),
    audio_buf_size(0),
    audio_buf_index(0)
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    screenCoords = graphics->getAbsoluteScreenCoords(guiOrientation, offsetX, offsetY, width, height);
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    Gui::getInstance()->addOverlay(this);
}

GuiVideo::~GuiVideo()
{
    delete buf[0];
    delete buf[1];
    delete buf[2];
    delete buf[3];
    Gui::getInstance()->removeOverlay(this);
}

void GuiVideo::load(const std::string & imageFileName)
{
	// Open file.
    if (av_open_input_file(&pFormatCtx, imageFileName.c_str(), NULL, 0, NULL) != 0)
	{
        fatalError("GuiVideo::load() : Couldn't open " + imageFileName);
    }

	// Retrieve stream information.
    if (av_find_stream_info(pFormatCtx) < 0)
	{
        fatalError("GuiVideo::load : Couldn't find stream information.");
    }

    //Find the first video and audio streams
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) 
    {
        //if (pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO) 
        //{
        //    videoStream = (int)i;
        //}
		if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO
			&& videoStream < 0) 
		{
				videoStream=i;
		}
		if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO &&
			audioStream < 0) 
		{
				audioStream=i;
		}
    }
    if (videoStream >= 0)
	{
        //fatalError("GuiVideo::load : Couldn't find a video stream.");
	    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    }
    if (audioStream >= 0)
	{
		// not a fatal error
//        fatalError("GuiVideo::load : Couldn't find a video stream.");
		aCodecCtx = pFormatCtx->streams[audioStream]->codec;
    }

    // Find the decoder for the video stream.
	if (videoStream >= 0)
	{
		AVCodec * pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
		if (pCodec == NULL) 
		{
			fatalError("GuiVideo::load : Unsupported codec.");
		}
		if (avcodec_open(pCodecCtx, pCodec) < 0)
		{
			fatalError("GuiVideo::load : Can not open video codec.");
		}
	}

    // Find the decoder for the audio stream.
	if (audioStream >= 0)
	{
		AVCodec * aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
		if(aCodec == NULL) 
		{
			fatalError("GuiVideo::load : Unsupported audio codec.");
		}
		//if (avcodec_open(aCodecCtx, aCodec) < 0)
		//{
		//	fatalError("GuiVideo::load : Can not open audio codec.");
		//}

		SDL_AudioSpec wanted_spec;
		SDL_AudioSpec obtained_spec;

		wanted_spec.freq = aCodecCtx->sample_rate;
		wanted_spec.format = AUDIO_S16SYS;
		wanted_spec.channels = aCodecCtx->channels;
		wanted_spec.silence = 0;
		wanted_spec.samples = 1024;           //SDL_AUDIO_BUFFER_SIZE;
		wanted_spec.callback = audio_callback;
		wanted_spec.userdata = this;

		if(SDL_OpenAudio(&wanted_spec, &obtained_spec) < 0) 
		{
			// Should we exit if no audio device is found?
			// I commented this out because I don't have a graphics card installed currently - dstover
			//fatalError("GuiVideo::load : SDL_OpenAudio() failed: " + std::string(SDL_GetError()));
		}

		// Try putting this above.
		if (avcodec_open(aCodecCtx, aCodec) < 0)
		{
			fatalError("GuiVideo::load : Can not open audio codec.");
		}

		memset(&audioq, 0, sizeof(PacketQueue));
		audioq.mutex = SDL_CreateMutex();
		audioq.cond = SDL_CreateCond();
		SDL_PauseAudio(0);
	}

    width = pCodecCtx->width;
    height = pCodecCtx->height;
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    screenCoords = graphics->getAbsoluteScreenCoords(guiOrientation, offsetX, offsetY, width, height);
 
	// TODO: Remove this hard-coding of bytesPerPixel.
	int bytesPerPixel = 3;
	if (GraphicsSystem::getInstance()->getGraphics()->usingDirect3D())
	{
		bytesPerPixel = 4;
	}

    for (int i = 0; i <  4; ++i ) 
    {
        buf[i] = new char[width * height * bytesPerPixel];
        if (buf[i] == NULL)
        {
            fatalError("GuiVideo::load : Memory allocation failure.");
        }
    }

    if (graphics->usingDirect3D())
    {
        textureSize = width * height * 4;
    }
    else
    {
	    textureSize = width * height * 3;
    }
	
    textureInfo = new TextureInfo(true, TextureInfo::RGB24_FORMAT);
    textureInfo->setWidth(width);
    textureInfo->setHeight(height);
    textureInfo->setPixelData(buf[head % 4]);
    textureReference = graphics->createTexture(textureInfo);

    unsigned short indices[6] = { 0, 1, 2, 2, 3, 0 };
	if (graphics->usingDirect3D())
	{
        Vertex4 aTriangle[ ] = 
		    { {static_cast<float>(screenCoords.left),  static_cast<float>(screenCoords.bottom),    0, 1, 0, 1},
		      {static_cast<float>(screenCoords.left),  static_cast<float>(screenCoords.top), 0, 1, 0, 0},
		      {static_cast<float>(screenCoords.right), static_cast<float>(screenCoords.top), 0, 1, 1, 0},
		      {static_cast<float>(screenCoords.right), static_cast<float>(screenCoords.bottom),    0, 1, 1, 1}};
	    vertexInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX4, 4);
	    vertexInfo->setVertexData(aTriangle);
	}
    else
    {
        Vertex1 aTriangle[ ] = 
		    { {static_cast<float>(screenCoords.left),  static_cast<float>(screenCoords.bottom),    0, 0, 1},
		      {static_cast<float>(screenCoords.left),  static_cast<float>(screenCoords.top), 0, 0, 0},
		      {static_cast<float>(screenCoords.right), static_cast<float>(screenCoords.top), 0, 1, 0},
		      {static_cast<float>(screenCoords.right), static_cast<float>(screenCoords.bottom),    0, 1, 1}};
	    vertexInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX1, 4);
	    vertexInfo->setVertexData(aTriangle);
    }
	vertexReference = graphics->createVertexBuffer(vertexInfo);
    IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, 6);
    ibInfo->setIndexData(indices);
    ibRef = graphics->createIndexBuffer(ibInfo);
}

void GuiVideo::start(bool isLooping)
{
    if (videoStream < 0)
    {
        fatalError("GuiVideo::start() : start() called without prior call to load().");
    }
    looping = isLooping;
    decodeThread = SDL_CreateThread(callDecode, reinterpret_cast<void *>(this));
    deadlineForNextFrame = SDL_GetTicks();
}

void GuiVideo::update()
{
}

void GuiVideo::draw()
{
	Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();

    Uint32 currentMilliseconds = SDL_GetTicks();
    if (currentMilliseconds >= deadlineForNextFrame && head < tail + 1 && !paused)
    {
        // TO DO:
        // Currently, we add 25 milliseconds to the deadline of the next frame.
        // Dr. Yu will determine a way to compute the video's frame rate so that
        // we can set this based on the video being played.
        deadlineForNextFrame += 25;
        ++head;
        textureReference->writeData(buf[head % 4], textureSize);
    }
    textureReference->activate();

    graphics->disableMode(Graphics::LIGHTING);
    graphics->disableMode(Graphics::CULLING);
    ibRef->activate();
    vertexReference->activate(false);
    graphics->drawIndexedTriangles(6, 0);
    vertexReference->deactivate();
    ibRef->deactivate();
    textureReference->deactivate();
}

int GuiVideo::packet_queue_put(AVPacket *pkt)
{
  AVPacketList *pkt1;
  if(av_dup_packet(pkt) < 0) {
    return -1;
  }
  pkt1 = ( AVPacketList *) av_malloc(sizeof(AVPacketList));
  if (!pkt1)
    return -1;
  pkt1->pkt = *pkt;
  pkt1->next = NULL;

  SDL_LockMutex(audioq.mutex);

  if (!audioq.last_pkt)
    audioq.first_pkt = pkt1;
  else
    audioq.last_pkt->next = pkt1;
  audioq.last_pkt = pkt1;
  audioq.nb_packets++;
  audioq.size += pkt1->pkt.size;
  SDL_CondSignal(audioq.cond);

  SDL_UnlockMutex(audioq.mutex);
  return 0;
}

//function to get packets from queue
int GuiVideo::packet_queue_get(AVPacket *pkt, int block) {
  AVPacketList *pkt1;
  int ret;

  SDL_LockMutex(audioq.mutex);

  for(;;) {

    if(quit) {
      ret = -1;
      break;
    }
    pkt1 = audioq.first_pkt;
    if (pkt1) {
      audioq.first_pkt = pkt1->next;
      if (!audioq.first_pkt)
        audioq.last_pkt = NULL;
      audioq.nb_packets--;
      audioq.size -= pkt1->pkt.size;
      *pkt = pkt1->pkt;
      av_free(pkt1);
      ret = 1;
      break;
    } else if (!block) {
      ret = 0;
      break;
    } else {
      SDL_CondWait(audioq.cond, audioq.mutex);
    }
  }
  SDL_UnlockMutex(audioq.mutex);
  return ret;
}


void GuiVideo::decode()
{
    // Allocate video frame
    AVFrame * pFrame = avcodec_alloc_frame();

    // Allocate an AVFrame structure
    AVFrame * pFrameRGB = avcodec_alloc_frame();
    if (pFrameRGB == NULL)
    {
        fatalError("GuiVideo::decode() : avcodec_alloc_frame() failed.");
    }

	int pixelFormat = PIX_FMT_RGB24;
	int bytesPerPixel = 3;
	if (GraphicsSystem::getInstance()->getGraphics()->usingDirect3D())
	{
		pixelFormat = PIX_FMT_RGB32;
		bytesPerPixel = 4;
	}
    // Allocate buffer.
    int numBytes = avpicture_get_size(pixelFormat, width, height) * sizeof(uint8_t);
    uint8_t * buffer = new uint8_t[numBytes];



    // Assign appropriate parts of buffer to image planes in pFrameRGB.
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset of AVPicture.
    avpicture_fill(
        reinterpret_cast<AVPicture *>(pFrameRGB), 
        buffer, 
        pixelFormat,
        width, 
        height);

    AVPacket packet;
    int got_picture;
    while (!quit) 
    {
        if (tail >= head + 4)
        {
            // Buffer is full.
            SDL_Delay(30);	
            continue;
        }

        // Produce data.
        if (av_read_frame(pFormatCtx, &packet) < 0) 
        {
            // error or end of file
            quit = true;
            break;
        }

		if (packet.stream_index == videoStream) 
		{
			// Decode video frame.
			if (avcodec_decode_video(pCodecCtx, pFrame, &got_picture, packet.data, packet.size) < 0)
			{
				fatalError("GuiVideo::decode() : avcodec_decode_video() failed.");
			}

			if (got_picture == 0) 
			{
				//av_free_packet(&packet);
				std::cout << "av_free_packet set got_picture to 0." << std::endl;
				continue;
				// I'm not sure if this is a failure or if means this frame should sinply be skipped.
				//fatalError("GuiVideo::decode() : avcodec_decode_video() set got_picture to 0.");
			}
			SwsContext * encoderSwsContext = sws_getContext(
				pCodecCtx->width, 
				pCodecCtx->height, 
				pCodecCtx->pix_fmt, 
				pCodecCtx->width, 
				pCodecCtx->height, 
				pixelFormat, 
				SWS_BICUBIC, 
				NULL, NULL, NULL); 

			sws_scale(
				encoderSwsContext, 
				pFrame->data,
				pFrame->linesize,
				0, 
				pCodecCtx->height, 
				pFrameRGB->data,
				pFrameRGB->linesize); 

			char * pb = buf[tail % 4];	
			int size = width * bytesPerPixel;
			for (int y = 0; y < height; y++) 
			{
				char * pc =  reinterpret_cast<char *>(pFrameRGB->data[0] + y * pFrameRGB->linesize[0]);
				int k = 0;
				char * p = pb + y * size;	
				for (int i = 0; i < width; ++i) 
				{
					p[k] = pc[k];   // red
					p[k+1] = pc[k+1];  // green
					p[k+2] = pc[k + 2]; // blue
				    if (pixelFormat == PIX_FMT_RGB24)
				    {
					    k += 3;
				    }
				    else // PIX_FMT_RGB32
			        {
				        p[k+3] = 0; // alpha
				        k += 4;
				    }
				}
			}
			tail++;
			//av_free_packet(&packet);
		}
		else if (packet.stream_index == audioStream) 
		{
			packet_queue_put(&packet);
        }
		else
		{
			av_free_packet(&packet);
		}
    }
    av_free(buffer);
    av_free(pFrameRGB);
    av_free(pFrame);
    avcodec_close(pCodecCtx);
    av_close_input_file(pFormatCtx);
}

void GuiVideo::unload()
{
    // Make is safe to call unload when nothing is loaded/playing.
    if (!quit)
    {
        quit = true;
        SDL_WaitThread(decodeThread, 0);
		//textureReference->destroy();
        //glDeleteBuffers(1, &pixelBufferObjectId);
    }
}
