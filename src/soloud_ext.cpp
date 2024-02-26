/*
SoLoud Extended
*/

#include "soloud.h"
#include "soloud_internal.h"
#include "soloud_ext.hpp"

namespace SoLoud
{

  void SoloudExt::stopAllNonProtected()
  {
    int i;
    lockAudioMutex();
    for (i = 0; i < (signed)mHighestVoice; i++)
    {
      if (!mVoice[i]) continue;
      if (mVoice[i]->mFlags & AudioSourceInstance::PROTECTED) continue;
      stopVoice(i);
    }
    unlockAudioMutex();
  }

}
