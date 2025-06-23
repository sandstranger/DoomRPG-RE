#include "DoomTranslator.h"
#include "DoomRPG.h"
#include "SDL_log.h"
#ifdef ANDROID
#include "Translator.h"
#endif
extern DoomRPG_t* doomRpg;

const char *get_translation(const char *input){
#ifdef ANDROID
    return doomRpg->enableMachineTextTranslation ? translate(input) : input;
#else
    return input;
#endif
}
