/*
========================================================================

                               DOOM RETRO
         The classic, refined DOOM source port. For Windows PC.

========================================================================

  Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
  Copyright (C) 2013-2015 Brad Harding.

  DOOM RETRO is a fork of CHOCOLATE DOOM by Simon Howard.
  For a complete list of credits, see the accompanying AUTHORS file.

  This file is part of DOOM RETRO.

  DOOM RETRO is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  DOOM RETRO is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DOOM RETRO. If not, see <http://www.gnu.org/licenses/>.

  DOOM is a registered trademark of id Software LLC, a ZeniMax Media
  company, in the US and/or other countries and is used without
  permission. All other trademarks are the property of their respective
  holders. DOOM RETRO is in no way affiliated with nor endorsed by
  id Software LLC.

========================================================================
*/

#if !defined(__S_SOUND__)
#define __S_SOUND__

#include "p_mobj.h"
#include "sounds.h"

typedef enum
{
    SNDDEVICE_NONE = 0,
    SNDDEVICE_PCSPEAKER = 1,
    SNDDEVICE_ADLIB = 2,
    SNDDEVICE_SB = 3,
    SNDDEVICE_PAS = 4,
    SNDDEVICE_GUS = 5,
    SNDDEVICE_WAVEBLASTER = 6,
    SNDDEVICE_SOUNDCANVAS = 7,
    SNDDEVICE_GENMIDI = 8,
    SNDDEVICE_AWE32 = 9,
} snddevice_t;

// Interface for sound modules
typedef struct
{
    // List of sound devices that this sound module is used for.
    snddevice_t *sound_devices;
    int num_sound_devices;

    // Initialize sound module
    // Returns true if successfully initialized
    boolean (*Init)(void);

    // Shutdown sound module
    void (*Shutdown)(void);

    // Returns the lump index of the given sound.
    int (*GetSfxLumpNum)(sfxinfo_t *sfxinfo);

    // Update the sound settings on the given channel.
    void (*UpdateSoundParams)(int channel, int vol, int sep);

    // Start a sound on a given channel.  Returns the channel id
    // or -1 on failure.
    int (*StartSound)(int id, int channel, int vol, int sep);

    // Stop the sound playing on the given channel.
    void (*StopSound)(int channel);

    // Query if a sound is playing on the given channel
    boolean (*SoundIsPlaying)(int channel);

} sound_module_t;

// Interface for music modules
typedef struct
{
    // List of sound devices that this music module is used for.
    snddevice_t *sound_devices;
    int num_sound_devices;

    // Initialize the music subsystem
    boolean (*Init)(void);

    // Shutdown the music subsystem
    void (*Shutdown)(void);

    // Set music volume - range 0-127
    void (*SetMusicVolume)(int volume);

    // Pause music
    void (*PauseMusic)(void);

    // Un-pause music
    void (*ResumeMusic)(void);

    // Register a song handle from data
    // Returns a handle that can be used to play the song
    void *(*RegisterSong)(void *data, int len);

    // Un-register (free) song data
    void (*UnRegisterSong)(void *handle);

    // Play the song
    void (*PlaySong)(void *handle, int looping);

    // Stop playing the current song.
    void (*StopSong)(void);

    // Query if music is playing.
    boolean (*MusicIsPlaying)(void);
} music_module_t;

extern int snd_sfxdevice;
extern int snd_musicdevice;
extern int snd_samplerate;

//
// Initializes sound stuff, including volume
// Sets channels, SFX and music volume,
//  allocates channel buffer, sets S_sfx lookup.
//
void S_Init(int sfxVolume, int musicVolume);

// Shut down sound
void S_Shutdown(void);

//
// Per level startup code.
// Kills playing sounds at start of level,
//  determines music if any, changes music.
//
void S_Start(void);

//
// Start sound for thing at <origin>
//  using <sound_id> from sounds.h
//
void S_StartSound(void *origin, int sound_id);

// Stop sound for thing at <origin>
void S_StopSound(mobj_t *origin);
void S_StopSounds(void);

// Start music using <music_id> from sounds.h
void S_StartMusic(int music_id);

// Start music using <music_id> from sounds.h,
//  and set whether looping
void S_ChangeMusic(int music_id, int looping, int cheating);

// query if music is playing
boolean S_MusicPlaying(void);

// Stops the music fer sure.
void S_StopMusic(void);

// Stop and resume music, during game PAUSE.
void S_PauseSound(void);
void S_ResumeSound(void);

//
// Updates music & sounds
//
void S_UpdateSounds(mobj_t *listener);

void S_SetMusicVolume(int volume);
void S_SetSfxVolume(int volume);

void I_InitTimidityConfig(void);

boolean I_AnySoundStillPlaying(void);

#endif
