/*
FluidSynth module for SoLoud - C API wrapper
*/

#include "soloud_fluidsynth.h"

using namespace SoLoud;

extern "C"
{

void FluidSynth_destroy(void * aClassPtr)
{
  delete (FluidSynth *)aClassPtr;
}

void * FluidSynth_create()
{
  return (void *)new FluidSynth;
}

int FluidSynth_load(void * aClassPtr, const char * aFilename)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  return cl->load(aFilename);
}

int FluidSynth_loadMem(void * aClassPtr, unsigned char * aMem, unsigned int aLength)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  return cl->loadMem(aMem, aLength);
}

int FluidSynth_loadMemEx(void * aClassPtr, unsigned char * aMem, unsigned int aLength, int aCopy, int aTakeOwnership)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  return cl->loadMem(aMem, aLength, !!aCopy, !!aTakeOwnership);
}

int FluidSynth_loadFile(void * aClassPtr, File * aFile)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  return cl->loadFile(aFile);
}

void FluidSynth_setVolume(void * aClassPtr, float aVolume)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->setVolume(aVolume);
}

void FluidSynth_setLooping(void * aClassPtr, int aLoop)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->setLooping(!!aLoop);
}

void FluidSynth_set3dMinMaxDistance(void * aClassPtr, float aMinDistance, float aMaxDistance)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dMinMaxDistance(aMinDistance, aMaxDistance);
}

void FluidSynth_set3dAttenuation(void * aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);
}

void FluidSynth_set3dDopplerFactor(void * aClassPtr, float aDopplerFactor)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dDopplerFactor(aDopplerFactor);
}

void FluidSynth_set3dProcessing(void * aClassPtr, int aDo3dProcessing)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dProcessing(!!aDo3dProcessing);
}

void FluidSynth_set3dListenerRelative(void * aClassPtr, int aListenerRelative)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dListenerRelative(!!aListenerRelative);
}

void FluidSynth_set3dDistanceDelay(void * aClassPtr, int aDistanceDelay)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dDistanceDelay(!!aDistanceDelay);
}

void FluidSynth_set3dCollider(void * aClassPtr, AudioCollider * aCollider)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dCollider(aCollider);
}

void FluidSynth_set3dColliderEx(void * aClassPtr, AudioCollider * aCollider, int aUserData)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dCollider(aCollider, aUserData);
}

void FluidSynth_set3dAttenuator(void * aClassPtr, AudioAttenuator * aAttenuator)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->set3dAttenuator(aAttenuator);
}

void FluidSynth_setInaudibleBehavior(void * aClassPtr, int aMustTick, int aKill)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->setInaudibleBehavior(!!aMustTick, !!aKill);
}

void FluidSynth_setFilter(void * aClassPtr, unsigned int aFilterId, Filter * aFilter)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->setFilter(aFilterId, aFilter);
}

void FluidSynth_stop(void * aClassPtr)
{
  FluidSynth * cl = (FluidSynth *)aClassPtr;
  cl->stop();
}

} // extern "C"
