
#include <SDL.h>
#include <stdio.h>

#include "soloud_c.h"
#include "fluidsynth/fluidsynth.h"
#include "DoomRPG.h"
#include "DoomCanvas.h"
#include "Sound.h"
#include "Menu.h"
#include "MenuSystem.h"
#include "SDL_Video.h"
#include "Z_Zip.h"
#include "Audio.h"

#define INIT_ALLSOUNDS	1

static soundTable[MAX_AUDIOFILES] = {
	5039, 5040, 5042, 5043, 5044, 5045, 5046, 5047, 5048, 5049, 5050,
	5051, 5052, 5053, 5054, 5055, 5057, 5058, 5059, 5060, 5061, 5062,
	5063, 5064, 5065, 5066, 5067, 5068, 5069, 5070, 5071, 5072, 5073,
	5074, 5076, 5077, 5078, 5079, 5080, 5081, 5082, 5083, 5084, 5085,
	5086, 5087, 5088, 5089, 5090, 5091, 5092, 5093, 5094, 5095, 5096,
	5097, 5098, 5099, 5100, 5101, 5102, 5103, 5104, 5105, 5106, 5107,
	5108, 5109, 5110, 5111, 5112, 5113, 5114, 5115, 5116, 5117, 5118,
	5119, 5120, 5121, 5122, 5123, 5124, 5125, 5126, 5127, 5128, 5129,
	5130, 5131, 5133, 5134, 5136, 5137, 5138
};

Sound_t* Sound_init(Sound_t* sound, DoomRPG_t* doomRpg)
{
	int i;

	printf("Sound_init\n");

	if (sound == NULL)
	{
		sound = SDL_malloc(sizeof(Sound_t));
		if (sound == NULL) {
			return NULL;
		}
	}
	SDL_memset(sound, 0, sizeof(Sound_t));

	sound->soundEnabled = 0;
	sound->volume = 100;
	sound->doomRpg = doomRpg;


	//Mix_VolumeMusic((sound->volume * MIX_MAX_VOLUME) / 100);

	//printf("Num Channes is: %d\n", Mix_AllocateChannels(-1));
	//printf("Music volume is: %d\n", Mix_VolumeMusic(-1));
	//printf("Sound volume is: %d\n", Mix_Volume(-1, -1));

#if INIT_ALLSOUNDS
	sound->audioFiles = SDL_malloc(sizeof(AudioFile_t) * MAX_AUDIOFILES);

	for (i = 0; i < MAX_AUDIOFILES; i++)
	{
		SDL_RWops* rw;
		char fileName[128];
		byte* fdata;
		int fSize;

		if ((i == 0) || (i == 1) || (i == 3)) { // Midi Files
			snprintf(fileName, sizeof(fileName), "%03d.mid", soundTable[i]);
			fdata = readZipFileEntry(fileName, &zipFile, &fSize);

			sound->audioFiles[i].ptr = FluidSynth_create();
			FluidSynth_loadMemEx(sound->audioFiles[i].ptr, fdata, fSize, false, true);
		}
		else {
			snprintf(fileName, sizeof(fileName), "%03d.wav", soundTable[i]);
			fdata = readZipFileEntry(fileName, &zipFile, &fSize);

			sound->audioFiles[i].ptr = Wav_create();
			Wav_loadMemEx(sound->audioFiles[i].ptr, fdata, fSize, false, true);
		}
	}
#endif

	return sound;
}

void Sound_free(Sound_t* sound, boolean freePtr)
{
#if INIT_ALLSOUNDS
	for (int i = 0; i < MAX_AUDIOFILES; i++) {
		if ((i == 0) || (i == 1) || (i == 3)) { // Midi Files
			FluidSynth_destroy(sound->audioFiles[i].ptr);
		}
		else {
			Wav_destroy(sound->audioFiles[i].ptr);
		}
	}
	SDL_free(sound->audioFiles);
#endif

	if (freePtr) {
		SDL_free(sound);
	}
}

void Sound_stopSounds(Sound_t* sound)
{
	Soloud_stopAll(audio.soloud);
}

void Sound_playSound(Sound_t* sound, int resourceID, byte flags, int priority)
{
	int id = Sound_getFromResourceID(resourceID);
	boolean isMusic = (flags & SND_FLG_ISMUSIC) ? true : false;
	boolean shouldLoop = (flags & SND_FLG_LOOP) ? true : false;

	if (sound->soundEnabled && id != -1) {
		if (flags & SND_FLG_STOPSOUNDS) Sound_stopSounds(sound);

		if (isMusic) {
			FluidSynth_setLooping(sound->audioFiles[id].ptr, shouldLoop);
		}
		else {
			Wav_setLooping(sound->audioFiles[id].ptr, shouldLoop);
		}

		int h = Soloud_play(audio.soloud, sound->audioFiles[id].ptr);
		if (flags & SND_FLG_NOFORCESTOP) Soloud_setProtectVoice(audio.soloud, h, true);
	}
}

int Sound_getFromResourceID(resourceID)
{
	for (int i = 0; i < MAX_AUDIOFILES; i++) {
		if (soundTable[i] == resourceID) {
			return i;
		}
	}

	return -1;
}

void Sound_updateVolume(Sound_t* sound)
{
	Soloud_setGlobalVolume(audio.soloud, (sound->volume / 100.0f));

	int menu = sound->doomRpg->menuSystem->menu;
	//if (menu == MENU_MAIN_OPTIONS || menu == MENU_INGAME_OPTIONS) { // Old
	if (menu == MENU_SOUND || menu == MENU_INGAME_SOUND) {
		Menu_textVolume(sound->doomRpg->menu, sound->volume);
	}
}

int Sound_minusVolume(Sound_t* sound, int volume)
{
	sound->volume -= volume;
	if (sound->volume < 0) {
		sound->volume = 0;
	}
	Sound_updateVolume(sound);
	return sound->volume;
}

int Sound_addVolume(Sound_t* sound, int volume)
{
	sound->volume += volume;
	if (sound->volume > 100) {
		sound->volume = 100;
	}
	Sound_updateVolume(sound);
	return sound->volume;
}