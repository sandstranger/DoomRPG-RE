/*
FluidSynth module for SoLoud
*/

#ifndef SOLOUD_FLSYNTH_H
#define SOLOUD_FLSYNTH_H

#include "fluidsynth/fluidsynth.h"
#include "soloud.h"

namespace SoLoud
{
  class FluidSynth;
  class File;

  class FluidSynthInstance : public AudioSourceInstance
  {
    FluidSynth* mParent;
    fluid_player_t* mPlayer;
    int mPlaying;

  public:
    FluidSynthInstance(FluidSynth* aParent);
    virtual ~FluidSynthInstance();
    virtual void getAudio(float *aBuffer, unsigned int aSamples);
    virtual bool hasEnded();
  };

  class FluidSynth : public AudioSource
  {
  public:
    char* mData;
    unsigned int mDataLen;
    FluidSynth();
    virtual ~FluidSynth();
    result load(const char* aFilename);
    result loadMem(unsigned char* aMem, unsigned int aLength, bool aCopy = false, bool aTakeOwnership = true);
    result loadFile(File* aFile);
    virtual AudioSourceInstance* createInstance();
  };
};

#endif