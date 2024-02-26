/*
SoLoud Extended - C API wrapper
*/

#include "soloud.h"
#include "soloud_ext.hpp"
#include "soloud_c_ext.h"

using namespace SoLoud;

extern "C"
{

  void* SoloudExt_create()
  {
    return (void*)new SoloudExt;
  }

  void SoloudExt_stopAllNonProtected(void* aClassPtr)
  {
    SoloudExt* cl = (SoloudExt*)aClassPtr;
    cl->stopAllNonProtected();
  }

}
