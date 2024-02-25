#ifndef SOUND_H__
#define SOUND_H__

struct DoomRPG_s;
struct Image_s;

#define MAX_AUDIOFILES		95

#define SND_FLG_LOOP		1	// Hace que el sonido se repita indefinidamente / Makes the sound repeat indefinitely
#define SND_FLG_STOPSOUNDS	2	// Detiene todos los sonidos / Stops all sounds
// New Flags
#define SND_FLG_NOFORCESTOP 4	// Evita que el canal sea detenido forzosamente / Prevents the channel from being forcibly stopped
#define SND_FLG_ISMUSIC		8	// Define el canal como una musica / Define the channel as a music

typedef struct AudioFile_s
{
	void* ptr;
} AudioFile_t;

typedef struct Sound_s
{
	boolean soundEnabled;
	int volume;
	struct DoomRPG_s* doomRpg;
	AudioFile_t* audioFiles; // New
} Sound_t;

Sound_t* Sound_init(Sound_t* sound, DoomRPG_t* doomRpg);
void Sound_free(Sound_t* sound, boolean freePtr);
void Sound_stopSounds(Sound_t* sound);
void Sound_playSound(Sound_t* sound, int resourceID, byte flags, int priority);
int Sound_getFromResourceID(int resourceID);
void Sound_updateVolume(Sound_t* sound);
int Sound_minusVolume(Sound_t* sound, int volume);
int Sound_addVolume(Sound_t* sound, int volume);

#endif
