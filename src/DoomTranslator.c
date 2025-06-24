#include "DoomTranslator.h"
#include "SDL_log.h"
#ifdef ANDROID
#include "Translator.h"
#endif
extern DoomRPG_t* doomRpg;

const char *get_translation(const char *input, boolean textFromDialog){
#ifdef ANDROID
    return doomRpg->enableMachineTextTranslation ? translate(input, textFromDialog) : input;
#else
    return input;
#endif
}
