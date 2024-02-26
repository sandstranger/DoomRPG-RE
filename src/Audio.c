#include <SDL.h>

#include "soloud_c.h"
#include "soloud_c_ext.h"
#include "fluidsynth/fluidsynth.h"
#include "DoomRPG.h"
#include "Audio.h"

#define SF_FILE "gm.sf2"

Audio_t audio;

void Audio_init(void)
{
  audio.soloud = SoloudExt_create();
  int res = Soloud_init(audio.soloud);
  if (res != 0) DoomRPG_Error("SoLoud init failed: %d", res);

  audio.fluidSynth.settings = new_fluid_settings();
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.gain", 0.6);
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.chorus.depth", 4.25);
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.chorus.level", 0.6);
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.chorus.speed", 0.2);
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.reverb.damp", 0.3);
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.reverb.level", 0.7);
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.reverb.room-size", 0.5);
  fluid_settings_setnum(audio.fluidSynth.settings, "synth.reverb.width", 0.8);

  audio.fluidSynth.synth = new_fluid_synth(audio.fluidSynth.settings);
  if (audio.fluidSynth.synth == NULL) DoomRPG_Error("FluidSynth init failed");

  if (fluid_is_soundfont(SF_FILE)) {
    fluid_synth_sfload(audio.fluidSynth.synth, SF_FILE, 1);
  } else {
    DoomRPG_Error("Cannot find the soundfont %s file", SF_FILE);
  }
}

void Audio_close(void)
{
  Soloud_deinit(audio.soloud);
  Soloud_destroy(audio.soloud);
  delete_fluid_synth(audio.fluidSynth.synth);
  delete_fluid_settings(audio.fluidSynth.settings);
}
