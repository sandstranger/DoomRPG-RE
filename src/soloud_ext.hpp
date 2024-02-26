/*
SoLoud Extended
*/

#ifndef SOLOUD_EXT_H
#define SOLOUD_EXT_H

namespace SoLoud
{
  class SoloudExt : Soloud
  {
  public:
    // Similar to built-in stopAll(), but skips protected sounds
    void stopAllNonProtected();
  };
}

#endif