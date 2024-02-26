/*
FluidSynth module for SoLoud
*/

#include "fluidsynth/fluidsynth.h"
#include "soloud_file.h"
#include "soloud_fluidsynth.hpp"
#include "Audio.h"

namespace SoLoud
{

  FluidSynthInstance::FluidSynthInstance(FluidSynth* aParent)
  {
    mParent = aParent;
    mPlayer = new_fluid_player(audio.fluidSynth.synth);
    fluid_player_add_mem(mPlayer, (const void*)mParent->mData, mParent->mDataLen);
    fluid_player_set_loop(mPlayer, (mParent->mFlags & AudioSource::SHOULD_LOOP) ? -1 : 1);
    mPlaying = fluid_player_play(mPlayer) == FLUID_OK;
  }

  void FluidSynthInstance::getAudio(float *aBuffer, unsigned int aSamples)
  {
    if (mPlayer == NULL) return;
    int s = aSamples;
    unsigned int outofs = 0;

    while (s && mPlaying)
    {
      int samples = 512;
      if (s < samples) samples = s;
      fluid_synth_write_float(audio.fluidSynth.synth, samples, aBuffer + outofs, 0, 1, aBuffer + outofs + aSamples, 0, 1);
      mPlaying = fluid_player_get_status(mPlayer) != FLUID_PLAYER_DONE;
      outofs += samples;
      s -= samples;
    }
  }

  bool FluidSynthInstance::hasEnded()
  {
    return !mPlaying;
  }

  FluidSynthInstance::~FluidSynthInstance()
  {
    if (mPlayer) delete_fluid_player(mPlayer);
    mPlayer = 0;
  }

  result FluidSynth::loadMem(unsigned char* aMem, unsigned int aLength, bool aCopy, bool aTakeOwnership)
  {
    MemoryFile mf;
    int res = mf.openMem(aMem, aLength, aCopy, aTakeOwnership);
    if (res != SO_NO_ERROR)
      return res;
    return loadFile(&mf);
  }

  result FluidSynth::load(const char* aFilename)
  {
    DiskFile df;
    int res = df.open(aFilename);
    if (res != SO_NO_ERROR)
      return res;
    return loadFile(&df);
  }

  result FluidSynth::loadFile(File* aFile)
  {
    if (mData)
    {
      delete[] mData;
    }

    mDataLen = aFile->length();
    mData = new char[mDataLen];
    if (!mData)
    {
      mData = 0;
      mDataLen = 0;
      return OUT_OF_MEMORY;
    }
    aFile->read((unsigned char*)mData, mDataLen);

    fluid_player_t* mpl = new_fluid_player(audio.fluidSynth.synth);
    int res = fluid_player_add_mem(mpl, (const void*)mData, mDataLen);
    if (res == FLUID_FAILED)
    {
      delete[] mData;
      mDataLen = 0;
      return FILE_LOAD_FAILED;
    }
    delete_fluid_player(mpl);
    return 0;
  }

  FluidSynth::FluidSynth()
  {
    mBaseSamplerate = 44100;
    mChannels = 2;
    mData = 0;
    mDataLen = 0;
  }

  FluidSynth::~FluidSynth()
  {
    stop();
    delete[] mData;
    mData = 0;
    mDataLen = 0;
  }

  AudioSourceInstance* FluidSynth::createInstance()
  {
    return new FluidSynthInstance(this);
  }

}