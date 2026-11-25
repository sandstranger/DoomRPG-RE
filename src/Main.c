//Using SDL and standard IO
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <zlib.h>

#include "Z_Zone.h"
#include "DoomRPG.h"
#include "DoomCanvas.h"
#include "Player.h"
#include "Hud.h"
#include "MenuSystem.h"
#include "SDL_Video.h"
#include "Z_Zip.h"

#ifdef ANDROID
#include <SDL_main.h>
#include "Sound.h"
#endif

extern DoomRPG_t* doomRpg;

#if ANDROID
// Set by the launcher through JNA, on the java side's thread; read on the game's with __atomic_load_n (atomic publication).
// A setter publishes a new copy and never frees the old one: the game may be using it.
static char *g_pathToZipFile = nullptr;
char *g_pathToUserFolder = nullptr;
char *g_pathToSDLControllerDB = nullptr;

static void freeChars(char **targetChars)
{
    if (targetChars)
        free(__atomic_exchange_n(targetChars, (char*)nullptr, __ATOMIC_ACQ_REL));
}

static void publishChars(char **targetChars, const char *value)
{
    char *copy = strdup(value ? value : "");    // (JNA passes null for a null String: strdup(null) crashed)
    __atomic_exchange_n(targetChars, copy, __ATOMIC_ACQ_REL);   // (the old copy is not freed: the game may still hold it)
}
#endif

#ifdef ANDROID
static int SDLCALL AndroidLifeCycleEventFilter(void*, SDL_Event* event);

int SDL_main(int argc, char **argv)
#else
int main(int argc, char* args[])
#endif
{
#ifdef ANDROID
    doomRpg = nullptr;
    {
        const char *userFolder = __atomic_load_n(&g_pathToUserFolder, __ATOMIC_ACQUIRE);
        if (userFolder)
            chdir(userFolder);
    }
#endif

	SDL_Event ev;
	int		UpTime = 0;
	int		mouseTime = 0;
	int		key, oldKey, mouse_Button;

	Z_Init();
	SDL_InitVideo();
	SDL_InitAudio();
#ifdef ANDROID
	SDL_AddEventWatch(AndroidLifeCycleEventFilter, nullptr);
    openZipFile(__atomic_load_n(&g_pathToZipFile, __ATOMIC_ACQUIRE), &zipFile);
#else
    openZipFile("DoomRPG.zip", &zipFile);
#endif

	/*int size;
	byte* data;
	data = readZipFileEntry("c.bmp", &zipFile, &size);

	SDL_RWops* rw;
	rw = SDL_RWFromFile("c.bmp", "w");
	SDL_RWwrite(rw, data, sizeof(byte), size);
	SDL_RWclose(rw);

	SDL_free(data);

	closeZipFile(&zipFile);*/

	if (DoomRPG_Init() == 0) {
		DoomRPG_Error("Failed to initialize Doom Rpg\n");
	}

	//Hud_addMessage(doomRpg->hud, "Bienvenido a Doom RPG por GEC...");

	const Uint8* state = SDL_GetKeyboardState(NULL);

	key = 0;
	oldKey = -1;

	while (doomRpg->closeApplet != true)
	{

		int currentTimeMillis = DoomRPG_GetUpTimeMS();

		mouse_Button = MOUSE_BUTTON_INVALID;

		while (SDL_PollEvent(&ev))
		{
			// check event type
			switch (ev.type) {

                case SDL_JOYDEVICEADDED:
                case SDL_CONTROLLERDEVICEADDED:
                    case SDL_JOYDEVICEREMOVED:
                case SDL_CONTROLLERDEVICEREMOVED:  {
                    RescanAndOpenFirstConnectedDevice();
                    break;
                }

				// Mouse Event
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				{
					Uint32 buttons = SDL_GetMouseState(NULL, NULL);

					if ((buttons & SDL_BUTTON_LMASK) != 0) {
						mouse_Button = MOUSE_BUTTON_LEFT;
					}
					else if ((buttons & SDL_BUTTON_MMASK) != 0) {
						mouse_Button = MOUSE_BUTTON_MIDDLE;
					}
					else if ((buttons & SDL_BUTTON_RMASK) != 0) {
						mouse_Button = MOUSE_BUTTON_RIGHT;
					}
					else if ((buttons & SDL_BUTTON_X1MASK) != 0) {
						mouse_Button = MOUSE_BUTTON_X1;
					}
					else if ((buttons & SDL_BUTTON_X2MASK) != 0) {
						mouse_Button = MOUSE_BUTTON_X2;
					}
					break;
				}

				case SDL_MOUSEWHEEL:
				{
					if (currentTimeMillis > mouseTime) {
						mouseTime = currentTimeMillis + 128;
						if (ev.wheel.y > 0) {
							mouse_Button = MOUSE_BUTTON_WHELL_UP;
						}
						else if (ev.wheel.y < 0) {
							mouse_Button = MOUSE_BUTTON_WHELL_DOWN;
						}
					}
					break;
				}

				case SDL_MOUSEMOTION:
				{
					if (!doomRpg->menuSystem->setBind) {
						if (currentTimeMillis > mouseTime) {
							mouseTime = currentTimeMillis + 128;
							int x = 0, y = 0;
							SDL_GetRelativeMouseState(&x, &y);

							int sensivity = (doomRpg->doomCanvas->mouseSensitivity * 1000) / 100;

							if (x <= -sensivity) {
								mouse_Button = MOUSE_BUTTON_MOTION_LEFT;
							}
							else if (x >= sensivity) {
								mouse_Button = MOUSE_BUTTON_MOTION_RIGHT;
							}

							if (doomRpg->doomCanvas->mouseYMove) {
								if (y <= -sensivity) {
									mouse_Button = MOUSE_BUTTON_MOTION_UP;
								}
								else if (y >= sensivity) {
									mouse_Button = MOUSE_BUTTON_MOTION_DOWN;
								}
							}
						}
					}
					break;
				}
				
				case SDL_WINDOWEVENT:
				{
					if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
						//printf("MESSAGE:Resizing window...\n");
						//SDL_Rect rect = { 0,0,640,480};
						//SDL_RenderSetViewport(sdlVideo.renderer, &rect);
						//resizeWindow();
					}

					if (ev.window.event == SDL_WINDOWEVENT_CLOSE) {
#if ANDROID
                        freeChars(&g_pathToZipFile);
                        freeChars(&g_pathToUserFolder);
                        freeChars(&g_pathToSDLControllerDB);
#endif
						SDL_Log("Window %d closed", ev.window.windowID);
						closeZipFile(&zipFile);
						DoomRPG_FreeAppData(doomRpg);
#ifdef ANDROID
						SDL_DelEventWatch(AndroidLifeCycleEventFilter, nullptr);	// (it uses the synth SDL_CloseAudio deletes)
#endif
						SDL_CloseAudio();
						SDL_Close();
						exit(0);
						break;
					}

					if (ev.window.event != SDL_WINDOWEVENT_CLOSE)
					{
						int w, h;
						SDL_GL_GetDrawableSize(sdlVideo.window, &w, &h);
						SDL_WarpMouseInWindow(sdlVideo.window, w / 2, h / 2);
						SDL_GetRelativeMouseState(NULL, NULL);
					}
					break;
				}

				case SDL_QUIT:
				{
					// shut down
					exit(0);
					break;
				}
			}

			key = DoomRPG_getEventKey(mouse_Button, state);

            if (key != oldKey) {
				//printf("oldKey %d\n", oldKey);
				//printf("key %d\n", key);

				oldKey = key;
				if (!doomRpg->menuSystem->setBind) {
					DoomCanvas_keyPressed(doomRpg->doomCanvas, key);
				}
				else {
					goto setBind;
				}
			}
			else if (key == 0) {
			setBind:
				if (doomRpg->menuSystem->setBind) {
					DoomRPG_setBind(doomRpg, mouse_Button, state);
				}
			}
		}
		
		if (currentTimeMillis > UpTime) {
			UpTime = currentTimeMillis + 15;
			DoomRPG_loopGame(doomRpg);
		}
	}
	closeZipFile(&zipFile);
	DoomRPG_FreeAppData(doomRpg);
#ifdef ANDROID
	SDL_DelEventWatch(AndroidLifeCycleEventFilter, nullptr);	// (it uses the synth SDL_CloseAudio deletes)
#endif
	SDL_CloseAudio();
	SDL_Close();
#if ANDROID
    freeChars(&g_pathToZipFile);
    freeChars(&g_pathToUserFolder);
    freeChars(&g_pathToSDLControllerDB);
#endif
	return 0;
}

#ifdef ANDROID
static bool soundWasPaused = false;

__attribute__((used)) __attribute__((visibility("default")))
void onNativeResume() {
}

// The lifecycle event watch runs on the java side's thread: only thread safe calls here. It paused the game's channels
// (Sound_pauseAll), reading and changing their flags and players while the game's thread played and stopped sounds. Now
// SDL_mixer's channels are paused (it locks the audio device itself), and the music, which fluidsynth plays through its
// own audio driver, is silenced through the synth's gain (its api is thread safe), given back from the settings on resume.
static void onResume() {
    double gain = 0;

    if (!__atomic_exchange_n(&soundWasPaused, false, __ATOMIC_ACQ_REL)) return;

    Mix_Resume(-1);
    if (fluidSynth.synth && fluidSynth.settings && fluid_settings_getnum(fluidSynth.settings, "synth.gain", &gain) == FLUID_OK)
        fluid_synth_set_gain(fluidSynth.synth, (float)gain);
}

__attribute__((used)) __attribute__((visibility("default")))
void onNativePause() {
}

static void onPause() {
    Mix_Pause(-1);
    if (fluidSynth.synth)
        fluid_synth_set_gain(fluidSynth.synth, 0.0f);
    __atomic_store_n(&soundWasPaused, true, __ATOMIC_RELEASE);
}

static int SDLCALL AndroidLifeCycleEventFilter(void*, SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_APP_WILLENTERBACKGROUND:
			onPause();
			break;
		case SDL_APP_DIDENTERFOREGROUND:
			onResume();
			break;
	}

	return 1;
}

// The queries return an int: JNA reads a java 'boolean' result as a 32 bit int, and of a returned C/C++ 'bool' only the
// low byte is defined on arm64 (AAPCS64 leaves the rest of the register unspecified).
__attribute__((used)) __attribute__((visibility("default")))
int needToShowScreenControls() {
    return 1;
}
__attribute__((used)) __attribute__((visibility("default")))
int needToInvokeMouseButtonsEvents(){
    return 1;
}
__attribute__((used)) __attribute__((visibility("default")))
int needToReInitGameControllers (){
    return 0;
}

__attribute__((used)) __attribute__((visibility("default")))
void setPathsToResources (const char *pathToArchive, const char *pathToUserFolder) {
    publishChars(&g_pathToUserFolder, pathToUserFolder);
    publishChars(&g_pathToZipFile, pathToArchive);
}

__attribute__((used)) __attribute__((visibility("default")))
void setPathToSDLControllerDB (const char *pathToSDLControllerDB){
    publishChars(&g_pathToSDLControllerDB, pathToSDLControllerDB);
}
#endif

