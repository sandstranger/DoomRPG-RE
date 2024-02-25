/*
FluidSynth module for SoLoud - C API wrapper
*/

#ifndef SOLOUD_C_FLSYNTH_H
#define SOLOUD_C_FLSYNTH_H

typedef void* FluidSynth;

void FluidSynth_destroy(void* aClassPtr);
void* FluidSynth_create();
int FluidSynth_load(void* aClassPtr, const char* aFilename);
int FluidSynth_loadMem(void* aClassPtr, unsigned char* aMem, unsigned int aLength);
int FluidSynth_loadMemEx(void* aClassPtr, unsigned char* aMem, unsigned int aLength, int aCopy, int aTakeOwnership);
int FluidSynth_loadFile(void* aClassPtr, File* aFile);
void FluidSynth_setVolume(void* aClassPtr, float aVolume);
void FluidSynth_setLooping(void* aClassPtr, int aLoop);
void FluidSynth_set3dMinMaxDistance(void* aClassPtr, float aMinDistance, float aMaxDistance);
void FluidSynth_set3dAttenuation(void* aClassPtr, unsigned int aAttenuationModel, float aAttenuationRolloffFactor);
void FluidSynth_set3dDopplerFactor(void* aClassPtr, float aDopplerFactor);
void FluidSynth_set3dProcessing(void* aClassPtr, int aDo3dProcessing);
void FluidSynth_set3dListenerRelative(void* aClassPtr, int aListenerRelative);
void FluidSynth_set3dDistanceDelay(void* aClassPtr, int aDistanceDelay);
void FluidSynth_set3dCollider(void* aClassPtr, AudioCollider* aCollider);
void FluidSynth_set3dColliderEx(void* aClassPtr, AudioCollider* aCollider, int aUserData);
void FluidSynth_set3dAttenuator(void* aClassPtr, AudioAttenuator* aAttenuator);
void FluidSynth_setInaudibleBehavior(void* aClassPtr, int aMustTick, int aKill);
void FluidSynth_setFilter(void* aClassPtr, unsigned int aFilterId, Filter* aFilter);
void FluidSynth_stop(void* aClassPtr);

#endif