/*
 *  SoundExperiment1.h
 *  Galileo
 *
 *  Created by David Stover on 1/11/09.
 *  Copyright 2009 MEK Graphics (Max Evan Kyle Technologies). All rights reserved.
 */

#include "SoundExperiment1.h"
#include "WaveReader.h"
#include "AIFFReader.h"

SoundExperiment1 *SoundExperiment1::theSound = 0;

SoundExperiment1::SoundExperiment1()
{
}

SoundExperiment1::~SoundExperiment1()
{
}

void SoundExperiment1::fillBuffer(int index, SoundData *soundData)
{// Load sound data
	ALenum format;  // AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8, AL_FORMAT_STEREO16
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;
    
	int nChannels = soundData->getNumChannels();
	int bitDepth = soundData->getBitsPerSample();
	int perSecond = soundData->getSamplesPerSecond();
	switch (nChannels)
	{
		case 1:
			if (bitDepth == 8)
			{
				format = AL_FORMAT_MONO8;
				break;
			}
			else if (bitDepth == 16)
			{
				format = AL_FORMAT_MONO16;
				break;
			}
		case 2:
			if (bitDepth == 8)
			{
				format = AL_FORMAT_STEREO8;
				break;
			}
			else if (bitDepth == 16)
			{
				format = AL_FORMAT_STEREO16;
				break;
			}
		default:
			return;
	}
	if (true)
	{
	    size = soundData->getDataSize();
	    printf("data size is %d\n", size);
	    freq = perSecond;
	    printf("freq = %d\n", freq);
	    data = soundData->getData();
	}
	else
	{
        //alutLoadWAVFile("boop.wav", &format, &data, &size, &freq, &loop);
	}
	
    alBufferData(buffers[index], format, data, size, freq);
    //alutUnloadWAV(format, data, size, freq);
    if (alGetError() != AL_NO_ERROR)
	{
		printf("OpenAL error trying to create the buffer\n");
		return;
	}
}

void SoundExperiment1::setup()
{
	// Initialization 
	ALCdevice *device = alcOpenDevice(NULL); // select the "preferred device" 
	
	if (device) { 
		ALCcontext *context=alcCreateContext(device, NULL); 
		alcMakeContextCurrent(context);  
	} 
	else
	{
		printf("cannot create device\n");
		return;
	}
	
	
	
	// Position of the source sound.
	ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
	
	// Velocity of the source sound.
	ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };
	
	
	// Position of the listener.
	ALfloat listenerPos[] = { 0.0, 0.0, 1.0 };
	
	// Velocity of the listener.
	ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
	
	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
	
	// Load sound data
	//ALenum format;  // AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8, AL_FORMAT_STEREO16
    //ALsizei size;
    //ALvoid* data;
    //ALsizei freq;
    //ALboolean loop;
	
    buffers = new ALuint[5];
    
	// Load wav data into a buffer.
	printf("About to load data into buffer.\n");
    alGenBuffers(5, buffers);
	printf("Checking for error\n");
    if (alGetError() != AL_NO_ERROR)
		return;
        //return AL_FALSE;
	
	WaveReader *wavReader = new WaveReader();
    AIFFReader *aiffReader = new AIFFReader();
	//SoundData *soundData = wavReader->getSoundData("boop.wav");
	//SoundData *soundData = aiffReader->getSoundData("note1.aif");
	SoundData *soundData = aiffReader->getSoundData("sounds/evan_ouch.aif");
    fillBuffer(0, soundData);
    soundData = aiffReader->getSoundData("sounds/kyle_boom.aif");
    fillBuffer(1, soundData);
    soundData = aiffReader->getSoundData("sounds/oi.aif");
    fillBuffer(2, soundData);
    soundData = aiffReader->getSoundData("sounds/explosion2.aif");
    fillBuffer(3, soundData);
    soundData = aiffReader->getSoundData("sounds/crazyday.aif");
    fillBuffer(4, soundData);
	
	// Bind buffer with a source.
	source = new ALuint[5];
    alGenSources(5, source);
	
    if (alGetError() == AL_INVALID_OPERATION)
	{
		printf("Invalid operation\n");
	}
	
	if (alGetError() != AL_NO_ERROR)
	{
		printf("OpenAL error trying to bind source\n");
		return;
	}
    //return AL_FALSE;
    bool loop = false;

	
    alSourcei (source[0], AL_BUFFER,   buffers[0]   );
    alSourcef (source[0], AL_PITCH,    1.0f     );
    alSourcef (source[0], AL_GAIN,     1.0f     );
    alSourcefv(source[0], AL_POSITION, sourcePos);
    alSourcefv(source[0], AL_VELOCITY, sourceVel);    
    alSourcei (source[0], AL_LOOPING,  loop     );
    
    alSourcei (source[1], AL_BUFFER,   buffers[1]   );
    alSourcef (source[1], AL_PITCH,    1.0f     );
    alSourcef (source[1], AL_GAIN,     1.0f     );
    alSourcefv(source[1], AL_POSITION, sourcePos);
    alSourcefv(source[1], AL_VELOCITY, sourceVel);
    alSourcei (source[1], AL_LOOPING,  loop     );
    
    alSourcei (source[2], AL_BUFFER,   buffers[2]   );
    alSourcef (source[2], AL_PITCH,    1.0f     );
    alSourcef (source[2], AL_GAIN,     1.0f     );
    alSourcefv(source[2], AL_POSITION, sourcePos);
    alSourcefv(source[2], AL_VELOCITY, sourceVel);
    alSourcei (source[2], AL_LOOPING,  loop     );
    
    alSourcei (source[3], AL_BUFFER,   buffers[3]   );
    alSourcef (source[3], AL_PITCH,    1.0f     );
    alSourcef (source[3], AL_GAIN,     1.0f     );
    alSourcefv(source[3], AL_POSITION, sourcePos);
    alSourcefv(source[3], AL_VELOCITY, sourceVel);
    alSourcei (source[3], AL_LOOPING,  loop     );
    
    alSourcei (source[4], AL_BUFFER,   buffers[4]   );
    alSourcef (source[4], AL_PITCH,    1.0f     );
    alSourcef (source[4], AL_GAIN,     1.0f     );
    alSourcefv(source[4], AL_POSITION, sourcePos);
    alSourcefv(source[4], AL_VELOCITY, sourceVel);
    alSourcei (source[4], AL_LOOPING,  loop     );
	
	// Set listener values.
    alListenerfv(AL_POSITION,    listenerPos);
    alListenerfv(AL_VELOCITY,    listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);
	
	// Clear the error state
	alGetError();
	printf("finished setup\n");
}

void SoundExperiment1::play(int sound)
{
        alSourceStop(source[sound]);
	    alSourcePlay(source[sound]);
}