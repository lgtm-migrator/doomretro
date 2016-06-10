/*
========================================================================

                           D O O M  R e t r o
         The classic, refined DOOM source port. For Windows PC.

========================================================================

  Copyright © 1993-2012 id Software LLC, a ZeniMax Media company.
  Copyright © 2013-2016 Brad Harding.

  DOOM Retro is a fork of Chocolate DOOM.
  For a list of credits, see the accompanying AUTHORS file.

  This file is part of DOOM Retro.

  DOOM Retro is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  DOOM Retro is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DOOM Retro. If not, see http://www.gnu.org/licenses/.

  DOOM is a registered trademark of id Software LLC, a ZeniMax Media
  company, in the US and/or other countries and is used without
  permission. All other trademarks are the property of their respective
  holders. DOOM Retro is in no way affiliated with nor endorsed by
  id Software.

========================================================================
*/

#include <ctype.h>

#if defined(WIN32)
#include <windows.h>
#include <ShellAPI.h>
#endif

#include "am_map.h"
#include "c_cmds.h"
#include "c_console.h"
#include "d_deh.h"
#include "doomstat.h"
#include "g_game.h"
#include "hu_stuff.h"
#include "i_gamepad.h"
#include "i_system.h"
#include "m_menu.h"
#include "m_misc.h"
#include "m_random.h"
#include "p_inter.h"
#include "p_local.h"
#include "p_setup.h"
#include "p_tick.h"
#include "s_sound.h"
#include "st_stuff.h"
#include "v_video.h"
#include "version.h"
#include "w_wad.h"
#include "z_zone.h"

#if !defined(MAX_PATH)
#define MAX_PATH                260
#endif

#define GIVECMDSHORTFORMAT      "<i>items</i>"
#define GIVECMDLONGFORMAT       "<b>ammo</b>|<b>armor</b>|<b>health</b>|<b>keys</b>|<b>weapons</b>|<b>all</b>|<i>item</i>"
#define KILLCMDFORMAT           "<b>player</b>|<b>all</b>|<i>monster</i>"
#define MAPCMDSHORTFORMAT       "<b>E</b><i>x</i><b>M</b><i>y</i>|<b>MAP</b><i>xy</i>"
#define MAPCMDLONGFORMAT        "<b>E</b><i>x</i><b>M</b><i>y</i>|<b>MAP</b><i>xy</i>|<b>first</b>|<b>previous</b>|<b>next</b>|<b>last</b>"
#define SPAWNCMDFORMAT          "<i>monster</i>|<i>item</i>"

int     ammo;
int     armor;
int     health;

extern dboolean         alwaysrun;
extern int              am_allmapcdwallcolor;
extern int              am_allmapfdwallcolor;
extern int              am_allmapwallcolor;
extern int              am_backcolor;
extern int              am_cdwallcolor;
extern dboolean         am_external;
extern int              am_fdwallcolor;
extern dboolean         am_grid;
extern int              am_gridcolor;
extern int              am_markcolor;
extern int              am_playercolor;
extern dboolean         am_rotatemode;
extern int              am_teleportercolor;
extern int              am_thingcolor;
extern int              am_tswallcolor;
extern int              am_wallcolor;
extern int              am_xhaircolor;
extern dboolean         autoload;
extern dboolean         centerweapon;
extern dboolean         con_obituaries;
extern dboolean         con_timestamps;
extern int              episode;
extern int              expansion;
extern int              facebackcolor;
extern int              gametime;
extern float            gp_deadzone_left;
extern float            gp_deadzone_right;
extern int              gp_sensitivity;
extern dboolean         gp_swapthumbsticks;
extern dboolean         gp_vibrate;
extern char             *iwadfolder;
extern dboolean         messages;
extern float            m_acceleration;
extern dboolean         m_doubleclick_use;
extern dboolean         m_novertical;
extern int              m_sensitivity;
extern int              m_threshold;
extern int              movebob;
extern char             *playername;
extern dboolean         r_althud;
extern int              r_berserkintensity;
extern int              r_blood;
extern int              r_bloodsplats_max;
extern int              r_bloodsplats_total;
extern dboolean         r_brightmaps;
extern dboolean         r_corpses_color;
extern dboolean         r_corpses_mirrored;
extern dboolean         r_corpses_moreblood;
extern dboolean         r_corpses_nudge;
extern dboolean         r_corpses_slide;
extern dboolean         r_corpses_smearblood;
extern int              r_detail;
extern int              r_diskicon;
extern dboolean         r_fixmaperrors;
extern dboolean         r_fixspriteoffsets;
extern dboolean         r_floatbob;
extern float            r_gamma;
extern dboolean         r_homindicator;
extern dboolean         r_hud;
extern dboolean         r_liquid_bob;
extern dboolean         r_liquid_clipsprites;
extern dboolean         r_liquid_current;
extern dboolean         r_liquid_lowerview;
extern dboolean         r_liquid_swirl;
extern char             *r_lowpixelsize;
extern dboolean         r_mirroredweapons;
extern dboolean         r_playersprites;
extern dboolean         r_rockettrails;
extern int              r_screensize;
extern dboolean         r_shadows;
extern int              r_shakescreen;
extern dboolean         r_translucency;
extern int              s_musicvolume;
extern dboolean         s_randommusic;
extern dboolean         s_randompitch;
extern int              s_sfxvolume;
extern char             *s_timiditycfgpath;
extern int              skilllevel;
extern unsigned int     stat_cheated;
extern unsigned int     stat_damageinflicted;
extern unsigned int     stat_damagereceived;
extern unsigned int     stat_deaths;
extern unsigned int     stat_itemspickedup;
extern unsigned int     stat_monsterskilled;
extern unsigned int     stat_monsterskilled_arachnotrons;
extern unsigned int     stat_monsterskilled_archviles;
extern unsigned int     stat_monsterskilled_baronsofhell;
extern unsigned int     stat_monsterskilled_cacodemons;
extern unsigned int     stat_monsterskilled_cyberdemons;
extern unsigned int     stat_monsterskilled_demons;
extern unsigned int     stat_monsterskilled_heavyweapondudes;
extern unsigned int     stat_monsterskilled_hellknights;
extern unsigned int     stat_monsterskilled_imps;
extern unsigned int     stat_monsterskilled_lostsouls;
extern unsigned int     stat_monsterskilled_mancubi;
extern unsigned int     stat_monsterskilled_painelementals;
extern unsigned int     stat_monsterskilled_revenants;
extern unsigned int     stat_monsterskilled_shotgunguys;
extern unsigned int     stat_monsterskilled_spectres;
extern unsigned int     stat_monsterskilled_spidermasterminds;
extern unsigned int     stat_monsterskilled_zombiemen;
extern unsigned int     stat_secretsrevealed;
extern unsigned int     stat_shotsfired;
extern unsigned int     stat_shotshit;
extern unsigned int     stat_time;
extern int              stillbob;
extern int              turbo;
extern dboolean         vid_capfps;
extern int              vid_display;
#if !defined(WIN32)
extern char             *vid_driver;
#endif
extern dboolean         vid_fullscreen;
extern char             *vid_scaledriver;
extern char             *vid_scalefilter;
extern char             *vid_screenresolution;
extern dboolean         vid_vsync;
extern dboolean         vid_widescreen;
extern char             *vid_windowposition;
extern char             *vid_windowsize;

extern int              pixelwidth;
extern int              pixelheight;
extern int              screenheight;
extern int              screenwidth;
extern char             gamekeyaction[NUMKEYS][256];

control_t controls[] =
{
    { "1",             keyboardcontrol, '1'             }, { "2",             keyboardcontrol, '2'             },
    { "3",             keyboardcontrol, '3'             }, { "4",             keyboardcontrol, '4'             },
    { "5",             keyboardcontrol, '5'             }, { "6",             keyboardcontrol, '6'             },
    { "7",             keyboardcontrol, '7'             }, { "8",             keyboardcontrol, '8'             },
    { "9",             keyboardcontrol, '9'             }, { "0",             keyboardcontrol, '0'             },
    { "-",             keyboardcontrol, KEY_MINUS       }, { "=",             keyboardcontrol, KEY_EQUALS      },
    { "+",             keyboardcontrol, KEY_EQUALS      }, { "backspace",     keyboardcontrol, KEY_BACKSPACE   },
    { "tab",           keyboardcontrol, KEY_TAB         }, { "q",             keyboardcontrol, 'q'             },
    { "w",             keyboardcontrol, 'w'             }, { "e",             keyboardcontrol, 'e'             },
    { "r",             keyboardcontrol, 'r'             }, { "t",             keyboardcontrol, 't'             },
    { "y",             keyboardcontrol, 'y'             }, { "u",             keyboardcontrol, 'u'             },
    { "i",             keyboardcontrol, 'i'             }, { "o",             keyboardcontrol, 'o'             },
    { "p",             keyboardcontrol, 'p'             }, { "[",             keyboardcontrol, '['             },
    { "]",             keyboardcontrol, ']'             }, { "enter",         keyboardcontrol, KEY_ENTER       },
    { "ctrl",          keyboardcontrol, KEY_RCTRL       }, { "a",             keyboardcontrol, 'a'             },
    { "s",             keyboardcontrol, 's'             }, { "d",             keyboardcontrol, 'd'             },
    { "f",             keyboardcontrol, 'f'             }, { "g",             keyboardcontrol, 'g'             },
    { "h",             keyboardcontrol, 'h'             }, { "j",             keyboardcontrol, 'j'             },
    { "k",             keyboardcontrol, 'k'             }, { "l",             keyboardcontrol, 'l'             },
    { ";",             keyboardcontrol, ';'             }, { "\'",            keyboardcontrol, '\''            },
    { "shift",         keyboardcontrol, KEY_RSHIFT      }, { "\\",            keyboardcontrol, '\\'            },
    { "z",             keyboardcontrol, 'z'             }, { "x",             keyboardcontrol, 'x'             },
    { "c",             keyboardcontrol, 'c'             }, { "v",             keyboardcontrol, 'v'             },
    { "b",             keyboardcontrol, 'b'             }, { "n",             keyboardcontrol, 'n'             },
    { "m",             keyboardcontrol, 'm'             }, { ",",             keyboardcontrol, ','             },
    { ".",             keyboardcontrol, '.'             }, { "/",             keyboardcontrol, '/'             },
    { "tilde",         keyboardcontrol, KEY_TILDE       }, { "alt",           keyboardcontrol, KEY_RALT        },
    { "space",         keyboardcontrol, ' '             }, { "numlock",       keyboardcontrol, KEY_NUMLOCK     },
    { "capslock",      keyboardcontrol, KEY_CAPSLOCK    }, { "scrolllock",    keyboardcontrol, KEY_SCRLCK      },
    { "home",          keyboardcontrol, KEY_HOME        }, { "up",            keyboardcontrol, KEY_UPARROW     },
    { "pageup",        keyboardcontrol, KEY_PGUP        }, { "left",          keyboardcontrol, KEY_LEFTARROW   },
    { "right",         keyboardcontrol, KEY_RIGHTARROW  }, { "end",           keyboardcontrol, KEY_END         },
    { "down",          keyboardcontrol, KEY_DOWNARROW   }, { "pagedown",      keyboardcontrol, KEY_PGDN        },
    { "insert",        keyboardcontrol, KEY_INS         }, { "printscreen",   keyboardcontrol, KEY_PRINTSCREEN },
    { "delete",        keyboardcontrol, KEY_DEL         }, { "escape",        keyboardcontrol, KEY_ESCAPE      },
    { "mouse1",        mousecontrol,    0               }, { "mouse2",        mousecontrol,    1               },
    { "mouse3",        mousecontrol,    2               }, { "mouse4",        mousecontrol,    3               },
    { "mouse5",        mousecontrol,    4               }, { "mouse6",        mousecontrol,    5               },
    { "mouse7",        mousecontrol,    6               }, { "mouse8",        mousecontrol,    7               },
    { "mouse9",        mousecontrol,    8               }, { "mouse10",       mousecontrol,    9               },
    { "mouse11",       mousecontrol,    10              }, { "mouse12",       mousecontrol,    11              },
    { "mouse13",       mousecontrol,    12              }, { "mouse14",       mousecontrol,    13              },
    { "mouse15",       mousecontrol,    14              }, { "mouse16",       mousecontrol,    15              },
    { "wheelup",       mousecontrol,    MOUSE_WHEELUP   }, { "wheeldown",     mousecontrol,    MOUSE_WHEELDOWN },
    { "dpadup",        gamepadcontrol,  1               }, { "dpaddown",      gamepadcontrol,  2               },
    { "dpadleft",      gamepadcontrol,  4               }, { "dpadright",     gamepadcontrol,  8               },
    { "start",         gamepadcontrol,  16              }, { "back",          gamepadcontrol,  32              },
    { "leftthumb",     gamepadcontrol,  64              }, { "rightthumb",    gamepadcontrol,  128             },
    { "leftshoulder",  gamepadcontrol,  256             }, { "rightshoulder", gamepadcontrol,  512             },
    { "lefttrigger",   gamepadcontrol,  1024            }, { "righttrigger",  gamepadcontrol,  2048            },
    { "gamepad1",      gamepadcontrol,  4096            }, { "gamepad2",      gamepadcontrol,  8192            },
    { "gamepad3",      gamepadcontrol,  16384           }, { "gamepad4",      gamepadcontrol,  32768           },
    { "",              0,               0               }
};

action_t actions[] =
{
    { "+alwaysrun",    &key_alwaysrun,          NULL,              &gamepadalwaysrun         },
    { "+automap",      &key_automap,            NULL,              &gamepadautomap           },
    { "+back",         &key_down,               NULL,              NULL                      },
    { "+back2",        &key_down2,              NULL,              NULL                      },
    { "+clearmark",    &key_automap_clearmark,  NULL,              &gamepadautomapclearmark  },
    { "+console",      &key_console,            NULL,              NULL                      },
    { "+fire",         &key_fire,               &mousebfire,       &gamepadfire              },
    { "+followmode",   &key_automap_followmode, NULL,              &gamepadautomapfollowmode },
    { "+forward",      &key_up,                 &mousebforward,    NULL                      },
    { "+forward2",     &key_up2,                NULL,              NULL                      },
    { "+grid",         &key_automap_grid,       NULL,              &gamepadautomapgrid       },
    { "+left",         &key_left,               NULL,              NULL                      },
    { "+mark",         &key_automap_mark,       NULL,              &gamepadautomapmark       },
    { "+maxzoom",      &key_automap_maxzoom,    NULL,              &gamepadautomapmaxzoom    },
    { "+menu",         &key_menu,               NULL,              &gamepadmenu              },
    { "+nextweapon",   &key_nextweapon,         &mousebnextweapon, &gamepadnextweapon        },
    { "+prevweapon",   &key_prevweapon,         &mousebprevweapon, &gamepadprevweapon        },
    { "+right",        &key_right,              NULL,              NULL                      },
    { "+rotatemode",   &key_automap_rotatemode, NULL,              &gamepadautomaprotatemode },
    { "+run",          &key_run,                &mousebrun,        &gamepadrun               },
    { "+screenshot",   &key_screenshot,         NULL,              NULL                      },
    { "+strafe",       &key_strafe,             &mousebstrafe,     NULL                      },
    { "+strafeleft",   &key_strafeleft,         NULL,              NULL                      },
    { "+strafeleft2",  &key_strafeleft2,        NULL,              NULL                      },
    { "+straferight",  &key_straferight,        NULL,              NULL                      },
    { "+straferight2", &key_straferight2,       NULL,              NULL                      },
    { "+use",          &key_use,                &mousebuse,        &gamepaduse               },
    { "+use2",         &key_use2,               NULL,              NULL                      },
    { "+weapon1",      &key_weapon1,            NULL,              &gamepadweapon1           },
    { "+weapon2",      &key_weapon2,            NULL,              &gamepadweapon2           },
    { "+weapon3",      &key_weapon3,            NULL,              &gamepadweapon3           },
    { "+weapon4",      &key_weapon4,            NULL,              &gamepadweapon4           },
    { "+weapon5",      &key_weapon5,            NULL,              &gamepadweapon5           },
    { "+weapon6",      &key_weapon6,            NULL,              &gamepadweapon6           },
    { "+weapon7",      &key_weapon7,            NULL,              &gamepadweapon7           },
    { "+zoomin",       &key_automap_zoomin,     NULL,              &gamepadautomapzoomin     },
    { "+zoomout",      &key_automap_zoomout,    NULL,              &gamepadautomapzoomout    },
    { "",              NULL,                    NULL,              NULL                      }
};

static dboolean cheat_func1(char *, char *, char *, char *);
static dboolean game_func1(char *, char *, char *, char *);
static dboolean null_func1(char *, char *, char *, char *);

static void clear_cmd_func2(char *, char *, char *, char *);
static void cmdlist_cmd_func2(char *, char *, char *, char *);
static void condump_cmd_func2(char *, char *, char *, char *);
static void cvarlist_cmd_func2(char *, char *, char *, char *);
static void endgame_cmd_func2(char *, char *, char *, char *);
static void exitmap_cmd_func2(char *, char *, char *, char *);
static dboolean fastmonsters_cmd_func1(char *, char *, char *, char *);
static void fastmonsters_cmd_func2(char *, char *, char *, char *);
static dboolean give_cmd_func1(char *, char *, char *, char *);
static void give_cmd_func2(char *, char *, char *, char *);
static dboolean god_cmd_func1(char *, char *, char *, char *);
static void god_cmd_func2(char *, char *, char *, char *);
static void help_cmd_func2(char *, char *, char *, char *);
static dboolean kill_cmd_func1(char *, char *, char *, char *);
static void kill_cmd_func2(char *, char *, char *, char *);
static dboolean load_cmd_func1(char *, char *, char *, char *);
static void load_cmd_func2(char *, char *, char *, char *);
static dboolean map_cmd_func1(char *, char *, char *, char *);
static void map_cmd_func2(char *, char *, char *, char *);
static void maplist_cmd_func2(char *, char *, char *, char *);
static void mapstats_cmd_func2(char *, char *, char *, char *);
static void noclip_cmd_func2(char *, char *, char *, char *);
static void nomonsters_cmd_func2(char *, char *, char *, char *);
static void notarget_cmd_func2(char *, char *, char *, char *);
static void pistolstart_cmd_func2(char *, char *, char *, char *);
static void playerstats_cmd_func2(char *, char *, char *, char *);
static void quit_cmd_func2(char *, char *, char *, char *);
static dboolean respawnmonsters_cmd_func1(char *, char *, char *, char *);
static void respawnmonsters_cmd_func2(char *, char *, char *, char *);
static dboolean resurrect_cmd_func1(char *, char *, char *, char *);
static void resurrect_cmd_func2(char *, char *, char *, char *);
static dboolean save_cmd_func1(char *, char *, char *, char *);
static void save_cmd_func2(char *, char *, char *, char *);
static dboolean spawn_cmd_func1(char *, char *, char *, char *);
static void spawn_cmd_func2(char *, char *, char *, char *);
static void thinglist_cmd_func2(char *, char *, char *, char *);
static void unbind_cmd_func2(char *, char *, char *, char *);

static dboolean bool_cvars_func1(char *, char *, char *, char *);
static void bool_cvars_func2(char *, char *, char *, char *);
static void color_cvars_func2(char *, char *, char *, char *);
static dboolean float_cvars_func1(char *, char *, char *, char *);
static void float_cvars_func2(char *, char *, char *, char *);
static dboolean int_cvars_func1(char *, char *, char *, char *);
static void int_cvars_func2(char *, char *, char *, char *);
static void str_cvars_func2(char *, char *, char *, char *);
static void time_cvars_func2(char *, char *, char *, char *);

static void am_external_cvar_func2(char *, char *, char *, char *);
static dboolean gp_deadzone_cvars_func1(char *, char *, char *, char *);
static void gp_deadzone_cvars_func2(char *, char *, char *, char *);
static void gp_sensitivity_cvar_func2(char *, char *, char *, char *);
static void player_cvars_func2(char *, char *, char *, char *);
static void playername_cvar_func2(char *, char *, char *, char *);
static void alwaysrun_cvar_func2(char *, char *, char *, char *);
static dboolean r_blood_cvar_func1(char *, char *, char *, char *);
static void r_blood_cvar_func2(char *, char *, char *, char *);
static dboolean r_detail_cvar_func1(char *, char *, char *, char *);
static void r_detail_cvar_func2(char *, char *, char *, char *);
static dboolean r_gamma_cvar_func1(char *, char *, char *, char *);
static void r_gamma_cvar_func2(char *, char *, char *, char *);
static void r_hud_cvar_func2(char *, char *, char *, char *);
static void r_lowpixelsize_cvar_func2(char *, char *, char *, char *);
static void r_screensize_cvar_func2(char *, char *, char *, char *);
static void r_translucency_cvar_func2(char *, char *, char *, char *);
static dboolean s_volume_cvars_func1(char *, char *, char *, char *);
static void s_volume_cvars_func2(char *, char *, char *, char *);
static dboolean turbo_cvar_func1(char *, char *, char *, char *);
static void turbo_cvar_func2(char *, char *, char *, char *);
static void vid_display_cvar_func2(char *, char *, char *, char *);
static void vid_fullscreen_cvar_func2(char *, char *, char *, char *);
static dboolean vid_scaledriver_cvar_func1(char *, char *, char *, char *);
static void vid_scaledriver_cvar_func2(char *, char *, char *, char *);
static dboolean vid_scalefilter_cvar_func1(char *, char *, char *, char *);
static void vid_scalefilter_cvar_func2(char *, char *, char *, char *);
static void vid_screenresolution_cvar_func2(char *, char *, char *, char *);
static void vid_showfps_cvar_func2(char *, char *, char *, char *);
static void vid_vsync_cvar_func2(char *, char *, char *, char *);
static void vid_widescreen_cvar_func2(char *, char *, char *, char *);
static void vid_windowposition_cvar_func2(char *, char *, char *, char *);
static void vid_windowsize_cvar_func2(char *, char *, char *, char *);

void C_Bind(char *, char *, char *, char *);

static int C_LookupValueFromAlias(const char *text, int aliastype)
{
    int i = 0;

    while (*aliases[i].text)
    {
        if (aliastype == aliases[i].type && M_StringCompare(text, aliases[i].text))
            return aliases[i].value;
        ++i;
    }
    return -1;
}

static char *C_LookupAliasFromValue(int value, alias_type_t aliastype)
{
    int i = 0;

    while (*aliases[i].text)
    {
        if (aliastype == aliases[i].type && value == aliases[i].value)
            return aliases[i].text;
        ++i;
    }
    return commify(value);
}

#define CMD(name, alt, cond, func, parms, form, desc) \
    { #name, #alt, cond, func, parms, CT_CMD, CF_NONE, NULL, 0, 0, 0, form, desc }
#define CMD_CHEAT(name, parms) \
    { #name, "", cheat_func1, NULL, parms, CT_CHEAT, CF_NONE, NULL, 0, 0, 0, "", "" }
#define CVAR_BOOL(name, alt, cond, func, desc) \
    { #name, #alt, cond, func, 1, CT_CVAR, CF_BOOLEAN, &name, 1, false, true, "", desc }
#define CVAR_INT(name, alt, cond, func, flags, aliases, desc) \
    { #name, #alt, cond, func, 1, CT_CVAR, (CF_INTEGER | flags), &name, aliases, name##_min, \
      name##_max, "", desc }
#define CVAR_FLOAT(name, alt, cond, func, flags, desc) \
    { #name, #alt, cond, func, 1, CT_CVAR, (CF_FLOAT | flags), &name, 0, 0, 0, "", desc }
#define CVAR_POS(name, alt, cond, func, desc) \
    { #name, #alt, cond, func, 1, CT_CVAR, CF_POSITION, &name, 0, 0, 0, "", desc }
#define CVAR_SIZE(name, alt, cond, func, desc) \
    { #name, #alt, cond, func, 1, CT_CVAR, CF_SIZE, &name, 0, 0, 0, "", desc }
#define CVAR_STR(name, alt, cond, func, desc) \
    { #name, #alt, cond, func, 1, CT_CVAR, CF_STRING, &name, 0, 0, 0, "", desc }
#define CVAR_TIME(name, alt, cond, func, desc) \
    { #name, #alt, cond, func, 1, CT_CVAR, (CF_TIME | CF_READONLY), &name, 0, 0, 0, "", desc }

consolecmd_t consolecmds[] =
{
    CVAR_BOOL(alwaysrun, "", bool_cvars_func1, alwaysrun_cvar_func2,
        "Toggles the player always running when moving."),
    CVAR_INT(am_allmapcdwallcolor, am_allmapcdwallcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of lines with a change in ceiling height in the automap\nwhen the player has the computer area map power-up."),
    CVAR_INT(am_allmapfdwallcolor, am_allmapfdwallcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of lines with a change in floor height in the automap\nwhen the player has the computer area map power-up."),
    CVAR_INT(am_allmapwallcolor, am_allmapwallcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of solid walls in the automap when the player has the\ncomputer area map power-up."),
    CVAR_INT(am_backcolor, am_backcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of the automap's background."),
    CVAR_INT(am_cdwallcolor, am_cdwallcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of lines with a change in ceiling height in the automap."),
    CVAR_BOOL(am_external, "", bool_cvars_func1, am_external_cvar_func2,
        "Toggles rendering of the automap on an external display."),
    CVAR_INT(am_fdwallcolor, am_fdwallcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of lines with a change in floor height in the automap."),
    CVAR_BOOL(am_followmode, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles follow mode in the automap."),
    CVAR_BOOL(am_grid, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the grid in the automap."),
    CVAR_INT(am_gridcolor, am_gridcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of the grid in the automap."),
    CVAR_INT(am_markcolor, am_markcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of marks in the automap."),
    CVAR_INT(am_playercolor, am_playercolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of the player in the automap."),
    CVAR_BOOL (am_rotatemode, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles rotate mode in the automap."),
    CVAR_INT(am_teleportercolor, am_teleportercolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of teleporters in the automap."),
    CVAR_INT(am_thingcolor, am_thingcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of things in the automap."),
    CVAR_INT(am_tswallcolor, am_tswallcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of lines with no change in height in the automap."),
    CVAR_INT(am_wallcolor, am_wallcolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of solid walls in the automap."),
    CVAR_INT(am_xhaircolor, am_xhaircolour, int_cvars_func1, color_cvars_func2, CF_NONE, NOALIAS,
        "The color of the crosshair in the automap."),
    CVAR_INT(ammo, "", game_func1, player_cvars_func2, CF_NONE, NOALIAS,
        "The player's ammo."),
    CVAR_INT(armor, armour, game_func1, player_cvars_func2, CF_PERCENT, NOALIAS,
        "The player's armor."),
    CVAR_BOOL(autoload, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles automatically loading the last savegame after the\nplayer dies."),
    CMD(bind, "", null_func1, C_Bind, 2, "[<i>control</i> [<b>+</b><i>action</i>]]",
        "Binds an action to a control."),
    CVAR_BOOL(centerweapon, centreweapon, bool_cvars_func1, bool_cvars_func2,
        "Toggles the centering of the player's weapon when firing."),
    CMD(clear, "", null_func1, clear_cmd_func2, 0, "",
        "Clears the console."),
    CMD(cmdlist, "", null_func1, cmdlist_cmd_func2, 1, "[<i>searchstring</i>]",
        "Shows a list of console commands."),
    CVAR_BOOL(con_obituaries, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles obituaries in the console when monsters are killed."),
    CVAR_BOOL(con_timestamps, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles timestamps in the console for player messages."),
    CMD(condump, "", null_func1, condump_cmd_func2, 1, "[<i>filename</i><b>.txt</b>]",
        "Dumps the console to a file."),
    CMD(cvarlist, "", null_func1, cvarlist_cmd_func2, 1, "[<i>searchstring</i>]",
        "Shows a list of console variables."),
    CMD(endgame, "", game_func1, endgame_cmd_func2, 0, "",
        "Ends a game."),
    CVAR_INT(episode, "", int_cvars_func1, int_cvars_func2, CF_NONE, NOALIAS,
        "The currently selected <i>DOOM</i> episode in the menu."),
    CMD(exitmap, "", game_func1, exitmap_cmd_func2, 0, "",
        "Exits the current map."),
    CVAR_INT(expansion, "", int_cvars_func1, int_cvars_func2, CF_NONE, NOALIAS,
        "The currently selected <i>DOOM II</i> expansion in the menu."),
    CVAR_INT(facebackcolor, facebackcolour, int_cvars_func1, int_cvars_func2, CF_NONE, NOALIAS,
        "The color behind the player's face in the status bar."),
    CMD(fastmonsters, "", game_func1, fastmonsters_cmd_func2, 1, "[<b>on</b>|<b>off</b>]",
        "Toggles fast monsters."),
    CVAR_TIME(gametime, "", null_func1, time_cvars_func2,
        "The amount of time since <i>"PACKAGE_NAME"</i> started."),
    CMD(give, "", give_cmd_func1, give_cmd_func2, 1, GIVECMDSHORTFORMAT,
        "Gives <b>ammo</b>, <b>armor</b>, <b>backpack</b>, <b>health</b>, <b>keys</b>, <b>weapons</b>, <b>all</b> or certain\n<i>items</i> to the player."),
    CMD(god, "", god_cmd_func1, god_cmd_func2, 1, "[<b>on</b>|<b>off</b>]",
        "Toggles god mode."),
    CVAR_FLOAT(gp_deadzone_left, "", gp_deadzone_cvars_func1, gp_deadzone_cvars_func2, CF_PERCENT,
        "The dead zone of the gamepad's left thumbstick."),
    CVAR_FLOAT(gp_deadzone_right, "", gp_deadzone_cvars_func1, gp_deadzone_cvars_func2, CF_PERCENT,
        "The dead zone of the gamepad's right thumbstick."),
    CVAR_INT(gp_sensitivity, "", int_cvars_func1, gp_sensitivity_cvar_func2, CF_NONE, NOALIAS,
        "The gamepad's sensitivity."),
    CVAR_BOOL(gp_swapthumbsticks, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles swapping the gamepad's left and right thumbsticks."),
    CVAR_BOOL(gp_vibrate, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles vibration for XInput gamepads."),
    CVAR_INT(health, "", game_func1, player_cvars_func2, CF_PERCENT, NOALIAS,
        "The player's health."),
#if defined(WIN32)
    CMD(help, "", null_func1, help_cmd_func2, 0, "",
        "Opens the <i>"PACKAGE_NAME" Wiki</i>."),
#else
    CMD(help, "", null_func1, help_cmd_func2, 0, "",
        "Opens the help screen."),
#endif
    CMD_CHEAT(idbeholda, 0),
    CMD_CHEAT(idbeholdl, 0),
    CMD_CHEAT(idbeholdi, 0),
    CMD_CHEAT(idbeholdr, 0),
    CMD_CHEAT(idbeholds, 0),
    CMD_CHEAT(idbeholdv, 0),
    CMD_CHEAT(idchoppers, 0),
    CMD_CHEAT(idclev, 1),
    CMD_CHEAT(idclip, 0),
    CMD_CHEAT(iddqd, 0),
    CMD_CHEAT(iddt, 0),
    CMD_CHEAT(idfa, 0),
    CMD_CHEAT(idkfa, 0),
    CMD_CHEAT(idmus, 1),
    CMD_CHEAT(idmypos, 0),
    CMD_CHEAT(idspispopd, 0),
    CVAR_STR(iwadfolder, "", null_func1, str_cvars_func2,
        "The folder where an IWAD was last opened."),
    CMD(kill, "", kill_cmd_func1, kill_cmd_func2, 1, KILLCMDFORMAT,
        "Kills the <b>player</b>, <b>all</b> monsters or a type of <i>monster</i>."),
    CMD(load, "", load_cmd_func1, load_cmd_func2, 1, "<i>filename</i><b>.save</b>",
        "Loads a game from a file."),
    CVAR_FLOAT(m_acceleration, "", float_cvars_func1, float_cvars_func2, CF_NONE,
        "The amount the mouse accelerates."),
    CVAR_BOOL(m_doubleclick_use, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles double-clicking a mouse button for the <b>+use</b> action."),
    CVAR_BOOL(m_novertical, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles no vertical movement of the mouse."),
    CVAR_INT(m_sensitivity, "", int_cvars_func1, int_cvars_func2, CF_NONE, NOALIAS,
        "The mouse's sensitivity."),
    CVAR_INT(m_threshold, "", int_cvars_func1, int_cvars_func2, CF_NONE, NOALIAS,
        "The mouse's acceleration threshold."),
    CMD(map, warp, map_cmd_func1, map_cmd_func2, 1, MAPCMDSHORTFORMAT,
        "Warps to a map."),
    CMD(maplist, "", null_func1, maplist_cmd_func2, 0, "",
        "Shows a list of the available maps."),
    CMD(mapstats, "", game_func1, mapstats_cmd_func2, 0, "",
        "Shows statistics about the current map."),
    CVAR_BOOL(messages, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles player messages."),
    CVAR_INT(movebob, "", int_cvars_func1, int_cvars_func2, CF_PERCENT, NOALIAS,
        "The amount the player bobs when moving."),
    CMD(noclip, "", game_func1, noclip_cmd_func2, 1, "[<b>on</b>|<b>off</b>]",
        "Toggles collision detection for the player."),
    CMD(nomonsters, "", null_func1, nomonsters_cmd_func2, 1, "[<b>on</b>|<b>off</b>]",
        "Toggles the presence of monsters in maps."),
    CMD(notarget, "", game_func1, notarget_cmd_func2, 1, "[<b>on</b>|<b>off</b>]",
        "Toggles the player as a target."),
    CMD(pistolstart, "", null_func1, pistolstart_cmd_func2, 1, "[<b>on</b>|<b>off</b>]",
        "Toggles the player starting each map with only a pistol."),
    CVAR_STR(playername, "", null_func1, playername_cvar_func2,
        "The name of the player used in player messages."),
    CMD(playerstats, "", null_func1, playerstats_cmd_func2, 0, "",
        "Shows statistics about the player."),
    CMD(quit, exit, null_func1, quit_cmd_func2, 0, "",
        "Quits <i>"PACKAGE_NAME"</i>."),
    CVAR_BOOL(r_althud, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the display of an alternate heads-up display when in\nwidescreen mode."),
    CVAR_INT(r_berserkintensity, "", int_cvars_func1, int_cvars_func2, CF_PERCENT, NOALIAS,
        "The intensity of the screen's red haze when the player has the\nberserk power-up and their fists selected."),
    CVAR_INT(r_blood, "", r_blood_cvar_func1, r_blood_cvar_func2, CF_NONE, BLOODALIAS,
        "The color of the blood of the player and monsters (<b>all</b>, <b>none</b> or\n<b>red</b>)."),
    CVAR_INT(r_bloodsplats_max, "", int_cvars_func1, int_cvars_func2, CF_NONE, NOALIAS,
        "The maximum number of blood splats allowed in a map (<b>0</b> to\n<b>1,048,576</b>)."),
    CVAR_INT(r_bloodsplats_total, "", int_cvars_func1, int_cvars_func2, CF_READONLY, NOALIAS,
        "The total number of blood splats in the current map."),
    CVAR_BOOL(r_brightmaps, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles brightmaps on certain wall textures."),
    CVAR_BOOL(r_corpses_color, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles corpses of marines having randomly colored Praetor\nsuits."),
    CVAR_BOOL(r_corpses_mirrored, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles corpses being randomly mirrored."),
    CVAR_BOOL(r_corpses_moreblood, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles blood splats around corpses that are spawned when\na map is loaded."),
    CVAR_BOOL(r_corpses_nudge, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles corpses being nudged when monsters walk over them."),
    CVAR_BOOL(r_corpses_slide, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles corpses reacting to barrel and rocket explosions."),
    CVAR_BOOL(r_corpses_smearblood, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles corpses producing blood splats as they slide."),
    CVAR_BOOL(r_detail, "", r_detail_cvar_func1, r_detail_cvar_func2,
        "Toggles the graphic detail (<b>low</b> or <b>high</b>)."),
    CVAR_BOOL(r_diskicon, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles showing a disk icon when loading and saving."),
    CVAR_BOOL(r_fixmaperrors, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the fixing of mapping errors in the <i>DOOM</i> and <i>DOOM II</i>\nIWADs."),
    CVAR_BOOL(r_fixspriteoffsets, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the fixing of sprite offsets."),
    CVAR_BOOL(r_floatbob, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles some power-ups bobbing up and down."),
    CVAR_FLOAT(r_gamma, "", r_gamma_cvar_func1, r_gamma_cvar_func2, CF_NONE,
        "The gamma correction level (<b>off</b> or <b>0.50</b> to <b>2.0</b>)."),
    CVAR_BOOL(r_homindicator, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the flashing Hall of Mirrors indicator."),
    CVAR_BOOL(r_hud, "", bool_cvars_func1, r_hud_cvar_func2,
        "Toggles the heads-up display when in widescreen mode."),
    CVAR_BOOL(r_liquid_bob, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the bobbing effect of liquid sectors."),
    CVAR_BOOL(r_liquid_clipsprites, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the bottom of sprites being clipped when in a liquid\nsector."),
    CVAR_BOOL(r_liquid_current, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles a slight current being applied to liquid sectors."),
    CVAR_BOOL(r_liquid_lowerview, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles lowering the player's view when in a liquid sector."),
    CVAR_BOOL(r_liquid_swirl, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the swirl effect of liquid sectors."),
    CVAR_SIZE(r_lowpixelsize, "", null_func1, r_lowpixelsize_cvar_func2,
        "The size of pixels when the graphic detail is low (<i>width</i><b>\xD7</b><i>height</i>)."),
    CVAR_BOOL(r_mirroredweapons, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles randomly mirroring the weapons dropped by monsters."),
    CVAR_BOOL(r_playersprites, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the display of the player's weapon."),
    CVAR_BOOL(r_rockettrails, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles the trails behind player and Cyberdemon rockets."),
    CVAR_INT(r_screensize, "", int_cvars_func1, r_screensize_cvar_func2, CF_NONE, NOALIAS,
        "The screen size (<b>0</b> to <b>8</b>)."),
    CVAR_BOOL(r_shadows, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles sprites casting shadows."),
    CVAR_INT(r_shakescreen, "", int_cvars_func1, int_cvars_func2, CF_PERCENT, NOALIAS,
        "The amount the screen shakes when the player is attacked."),
    CVAR_BOOL(r_translucency, "", bool_cvars_func1, r_translucency_cvar_func2,
        "Toggles the translucency of sprites and textures."),
    CMD(respawnmonsters, "", respawnmonsters_cmd_func1, respawnmonsters_cmd_func2, 1, "[<b>on</b>|<b>off</b>]",
        "Toggles respawning monsters."),
    CMD(resurrect, "", resurrect_cmd_func1, resurrect_cmd_func2, 0, "",
        "Resurrects the player."),
    CVAR_INT(s_musicvolume, "", s_volume_cvars_func1, s_volume_cvars_func2, CF_PERCENT, NOALIAS,
        "The music volume."),
    CVAR_BOOL(s_randommusic, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles randomizing the music at the start of each map."),
    CVAR_BOOL(s_randompitch, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles randomizing the pitch of monster sound effects."),
    CVAR_INT(s_sfxvolume, "", s_volume_cvars_func1, s_volume_cvars_func2, CF_PERCENT, NOALIAS,
        "The sound effects volume."),
    CVAR_STR(s_timiditycfgpath, "", null_func1, str_cvars_func2,
        "The path of Timidity's configuration file."),
    CMD(save, "", save_cmd_func1, save_cmd_func2, 1, "<i>filename</i><b>.save</b>",
        "Saves the game to a file."),
    CVAR_INT(skilllevel, "", int_cvars_func1, int_cvars_func2, CF_NONE, NOALIAS,
        "The currently selected skill level in the menu."),
    CMD(spawn, summon, spawn_cmd_func1, spawn_cmd_func2, 1, SPAWNCMDFORMAT,
        "Spawns a <i>monster</i> or <i>item</i>."),
    CVAR_INT(stillbob, "", int_cvars_func1, int_cvars_func2, CF_PERCENT, NOALIAS,
        "The amount the player bobs when still."),
    CMD(thinglist, "", game_func1, thinglist_cmd_func2, 0, "",
        "Shows a list of things in the current map."),
    CVAR_INT(turbo, "", turbo_cvar_func1, turbo_cvar_func2, CF_PERCENT, NOALIAS,
        "The speed of the player (<b>10%</b> to <b>400%</b>)."),
    CMD(unbind, "", null_func1, unbind_cmd_func2, 1, "<i>control</i>",
        "Unbinds the action from a control."),
    CVAR_BOOL(vid_capfps, "", bool_cvars_func1, bool_cvars_func2,
        "Toggles capping of the framerate at 35 FPS."),
    CVAR_INT(vid_display, "", int_cvars_func1, vid_display_cvar_func2, CF_NONE, NOALIAS,
        "The display used to render the game."),
#if !defined(WIN32)
    CVAR_STR(vid_driver, "", null_func1, str_cvars_func2,
        "The video driver used to render the game."),
#endif
    CVAR_BOOL(vid_fullscreen, "", bool_cvars_func1, vid_fullscreen_cvar_func2,
        "Toggles between fullscreen and a window."),
    CVAR_STR(vid_scaledriver, "", vid_scaledriver_cvar_func1, vid_scaledriver_cvar_func2,
        "The driver used to scale the display (<b>\"\"</b>, <b>\"direct3d\"</b>, <b>\"opengl\"</b>\nor <b>\"software\"</b>)."),
    CVAR_STR(vid_scalefilter, "", vid_scalefilter_cvar_func1, vid_scalefilter_cvar_func2,
        "The filter used to scale the display (<b>\"nearest\"</b>, <b>\"linear\"</b> or\n<b>\"nearest_linear\"</b>)."),
    CVAR_SIZE(vid_screenresolution, "", null_func1, vid_screenresolution_cvar_func2,
        "The screen's resolution when fullscreen (<b>desktop</b> or\n<i>width</i><b>\xD7</b><i>height</i>)."),
    CVAR_BOOL(vid_showfps, "", bool_cvars_func1, vid_showfps_cvar_func2,
        "Toggles the display of the average frames per second."),
    CVAR_BOOL(vid_vsync, "", bool_cvars_func1, vid_vsync_cvar_func2,
        "Toggles vertical synchronization with the display's refresh\nrate."),
    CVAR_BOOL(vid_widescreen, "", bool_cvars_func1, vid_widescreen_cvar_func2,
        "Toggles widescreen mode."),
    CVAR_POS(vid_windowposition, "", null_func1, vid_windowposition_cvar_func2,
        "The position of the window on the desktop (<b>centered</b> or <b>(</b><i>x</i><b>,</b><i>y</i><b>)</b>)."),
    CVAR_SIZE(vid_windowsize, "", null_func1, vid_windowsize_cvar_func2,
        "The size of the window on the desktop (<i>width</i><b>\xD7</b><i>height</i>)."),

    { "", "", null_func1, NULL, 0, 0, CF_NONE, NULL, 0, 0, 0, "", "" }
};

static dboolean cheat_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (gamestate != GS_LEVEL)
        return false;
    else if (M_StringCompare(cmd, cheat_god.sequence))
        return (gameskill != sk_nightmare);
    else if (M_StringCompare(cmd, cheat_ammonokey.sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_ammo.sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_mus.sequence))
        return (!nomusic && musicVolume);
    else if (M_StringCompare(cmd, cheat_noclip.sequence))
        return (gamemode != commercial && gameskill != sk_nightmare);
    else if (M_StringCompare(cmd, cheat_commercial_noclip.sequence))
        return (gamemode == commercial && gameskill != sk_nightmare);
    else if (M_StringCompare(cmd, cheat_powerup[0].sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_powerup[1].sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_powerup[2].sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_powerup[3].sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_powerup[4].sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_powerup[5].sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_powerup[6].sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_choppers.sequence))
        return (gameskill != sk_nightmare && players[0].health > 0);
    else if (M_StringCompare(cmd, cheat_mypos.sequence))
        return true;
    else if (M_StringCompare(cmd, cheat_clev.sequence))
    {
        char   lump[6];
        int    map;

        if (gamemode == commercial)
        {
            map = (parm1[0] - '0') * 10 + parm1[1] - '0';
            M_snprintf(lump, sizeof(lump), "MAP%c%c", parm1[0], parm1[1]);
        }
        else
        {
            map = parm1[1] - '0';
            M_snprintf(lump, sizeof(lump), "E%cM%c", parm1[0], parm1[1]);
        }

        return (W_CheckNumForName(lump) >= 0 && (gamemission != pack_nerve || map <= 9)
            && (!BTSX || W_CheckMultipleLumps(lump) > 1));
    }
    else if (M_StringCompare(cmd, cheat_amap.sequence))
        return (automapactive || mapwindow);
    return false;
}

static dboolean game_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (gamestate == GS_LEVEL);
}

static dboolean null_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return true;
}

//
// bind cmd
//
static void C_DisplayBinds(char *action, int value, controltype_t type, int count)
{
    int i = 0;
    int tabs[8] = { 40, 130, 0, 0, 0, 0, 0, 0 };

    while (controls[i].type)
    {
        if (controls[i].type == type && controls[i].value == value)
        {
            char *control = controls[i].control;

            if (strlen(control) == 1)
                C_TabbedOutput(tabs, "%i.\t\'%s\'\t%s", count, (control[0] == '=' ? "+" : control),
                    action);
            else
                C_TabbedOutput(tabs, "%i.\t%s\t%s", count, control, action);
            break;
        }
        ++i;
    }
}

static void C_UnbindDuplicates(int keep, controltype_t type, int value)
{
    int i = 0;

    while (*actions[i].action)
    {
        if (i != keep)
        {
            if (type == keyboardcontrol && actions[i].keyboard
                && value == *(int *)actions[i].keyboard)
                *(int *)actions[i].keyboard = 0;
            else if (type == mousecontrol && actions[i].mouse && value == *(int *)actions[i].mouse)
                *(int *)actions[i].mouse = -1;
            else if (type == gamepadcontrol && actions[i].gamepad
                && value == *(int *)actions[i].gamepad)
                *(int *)actions[i].gamepad = 0;
        }

        ++i;
    }
    M_SaveCVARs();
}

void C_Bind(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (!*parm1)
    {
        int     action = 0;
        int     count = 1;

        while (*actions[action].action)
        {
            if (actions[action].keyboard)
                C_DisplayBinds(actions[action].action, *(int *)actions[action].keyboard,
                    keyboardcontrol, count++);
            if (actions[action].mouse)
                C_DisplayBinds(actions[action].action, *(int *)actions[action].mouse, mousecontrol,
                    count++);
            if (actions[action].gamepad)
                C_DisplayBinds(actions[action].action, *(int *)actions[action].gamepad,
                    gamepadcontrol, count++);
            ++action;
        }
    }
    else
    {
        int     i = 0;

        while (controls[i].type)
        {
            if (M_StringCompare(parm1, controls[i].control))
                break;
            ++i;
        }

        if (*controls[i].control)
        {
            int action = 0;

            if (!*parm2)
            {
                while (*actions[action].action)
                {
                    if (controls[i].type == keyboardcontrol && actions[action].keyboard
                        && controls[i].value == *(int *)actions[action].keyboard)
                        C_Output(actions[action].action);
                    else if (controls[i].type == mousecontrol && actions[action].mouse
                        && controls[i].value == *(int *)actions[action].mouse)
                        C_Output(actions[action].action);
                    else if (controls[i].type == gamepadcontrol && actions[action].gamepad
                        && controls[i].value == *(int *)actions[action].gamepad)
                        C_Output(actions[action].action);
                    ++action;
                }
            }
            else if (M_StringCompare(parm2, "none"))
            {
                while (*actions[action].action)
                {
                    switch (controls[i].type)
                    {
                        case keyboardcontrol:
                            if (actions[action].keyboard
                                && controls[i].value == *(int *)actions[action].keyboard)
                            {
                                *(int *)actions[action].keyboard = 0;
                                M_SaveCVARs();
                            }
                            break;

                        case mousecontrol:
                            if (actions[action].mouse
                                && controls[i].value == *(int *)actions[action].mouse)
                            {
                                *(int *)actions[action].mouse = -1;
                                M_SaveCVARs();
                            }
                            break;

                        case gamepadcontrol:
                            if (actions[action].gamepad
                                && controls[i].value == *(int *)actions[action].gamepad)
                            {
                                *(int *)actions[action].gamepad = 0;
                                M_SaveCVARs();
                            }
                            break;
                    }
                    ++action;
                }
            }
            else
            {
                dboolean        bound = false;

                while (*actions[action].action)
                {
                    if (M_StringCompare(parm2, actions[action].action))
                        break;
                    ++action;
                }

                if (*actions[action].action)
                {
                    switch (controls[i].type)
                    {
                        case keyboardcontrol:
                            if (actions[action].keyboard)
                            {
                                *(int *)actions[action].keyboard = controls[i].value;
                                bound = true;
                                C_UnbindDuplicates(action, keyboardcontrol, controls[i].value);
                            }
                            break;

                        case mousecontrol:
                            if (actions[action].mouse)
                            {
                                *(int *)actions[action].mouse = controls[i].value;
                                bound = true;
                                C_UnbindDuplicates(action, mousecontrol, controls[i].value);
                            }
                            break;

                        case gamepadcontrol:
                            if (actions[action].gamepad)
                            {
                                *(int *)actions[action].gamepad = controls[i].value;
                                bound = true;
                                C_UnbindDuplicates(action, gamepadcontrol, controls[i].value);
                            }
                            break;
                    }

                    if (*cmd)
                        M_SaveCVARs();
                }

                if (!bound)
                    C_Warning("The %s action can't be bound to %s.", parm2, controls[i].control);
            }
        }
    }
}

//
// clear cmd
//
extern int      consolestrings;

static void clear_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    consolestrings = 0;
    C_Output("");
}

//
// cmdlist cmd
//
static void cmdlist_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;
    int count = 1;
    int tabs[8] = { 40, 204, 0, 0, 0, 0, 0, 0 };

    while (*consolecmds[i].name)
    {
        if (consolecmds[i].type == CT_CMD && *consolecmds[i].description
            && (!*parm1 || wildcard(consolecmds[i].name, parm1)))
        {
            char        description1[255];
            char        description2[255] = "";
            char        *p;

            M_StringCopy(description1, consolecmds[i].description, 255);

            if ((p = strchr(description1, '\n')))
            {
                *p = '\0';
                ++p;
                M_StringCopy(description2, p, 255);
            }

            C_TabbedOutput(tabs, "%i.\t<b>%s</b> %s\t%s", count++, consolecmds[i].name,
                consolecmds[i].format, description1);

            if (*description2)
                C_TabbedOutput(tabs, "\t\t%s", description2);
        }
        ++i;
    }
}

//
// condump cmd
//
static void condump_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (consolestrings)
    {
        char            filename[MAX_PATH];
        FILE            *file;
        const char      *appdatafolder = M_GetAppDataFolder();

        M_MakeDirectory(appdatafolder);
        
        if (!*parm1)
        {
            int count = 0;

            M_snprintf(filename, sizeof(filename), "%s"DIR_SEPARATOR_S"condump.txt",
                appdatafolder);
            while (M_FileExists(filename))
                M_snprintf(filename, sizeof(filename), "%s"DIR_SEPARATOR_S"condump (%i).txt",
                    appdatafolder, ++count);
        }
        else
            M_snprintf(filename, sizeof(filename), "%s"DIR_SEPARATOR_S"%s", appdatafolder, parm1);

        file = fopen(filename, "wt");

        if (file)
        {
            int i;

            for (i = 1; i < consolestrings - 1; ++i)
                if (console[i].type == dividerstring)
                    fprintf(file, "%s\n", DIVIDERSTRING);
                else
                {
                    unsigned int        inpos;
                    unsigned int        spaces;
                    char                *string = strdup(console[i].string);
                    unsigned int        len;
                    unsigned int        outpos = 0;
                    int                 tabcount = 0;

                    strreplace(string, "<b>", "");
                    strreplace(string, "</b>", "");
                    strreplace(string, "<i>", "");
                    strreplace(string, "</i>", "");
                    len = strlen(string);

                    for (inpos = 0; inpos < len; ++inpos)
                    {
                        char    ch = string[inpos];

                        if (ch != '\n')
                            if (ch == '\t')
                            {
                                unsigned int    tabstop = console[i].tabs[tabcount] / 5;

                                if (outpos < tabstop)
                                {
                                    for (spaces = 0; spaces < tabstop - outpos; ++spaces)
                                        fputc(' ', file);
                                    outpos = tabstop;
                                    ++tabcount;
                                }
                                else
                                {
                                    fputc(' ', file);
                                    ++outpos;
                                }
                            }
                            else if (ch != '~')
                            {
                                fputc(ch, file);
                                ++outpos;
                            }
                    }

                    if (con_timestamps && *console[i].timestamp)
                    {
                        for (spaces = 0; spaces < 91 - outpos; ++spaces)
                            fputc(' ', file);
                        fputs(console[i].timestamp, file);
                    }

                    fputc('\n', file);
                }

            fclose(file);

            C_Output("Dumped the console to the file <b>%s</b>.", filename);
        }
    }
}

//
// cvarlist cmd
//
static void cvarlist_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;
    int count = 1;
    int tabs[8] = { 35, 179, 257, 0, 0, 0, 0, 0 };

    while (*consolecmds[i].name)
    {
        if (consolecmds[i].type == CT_CVAR && (!*parm1 || wildcard(consolecmds[i].name, parm1)))
        {
            char        description1[255];
            char        description2[255] = "";
            char        *p;

            M_StringCopy(description1, consolecmds[i].description, 255);

            if ((p = strchr(description1, '\n')))
            {
                *p = '\0';
                ++p;
                M_StringCopy(description2, p, 255);
            }

            if (M_StringCompare(consolecmds[i].name, stringize(ammo)))
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%i</b>\t%s", count++, consolecmds[i].name,
                    (gamestate == GS_LEVEL ? players[0].ammo[weaponinfo[players[0].readyweapon].ammo] : 0),
                    description1);
            else if (M_StringCompare(consolecmds[i].name, stringize(armor)))
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%i%%</b>\t%s", count++, consolecmds[i].name,
                    (gamestate == GS_LEVEL ? players[0].armorpoints : 0), description1);
            else if (M_StringCompare(consolecmds[i].name, stringize(health)))
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%i%%</b>\t%s", count++, consolecmds[i].name,
                    (gamestate == GS_LEVEL ? players[0].health : 0), description1);
            else if (consolecmds[i].flags & CF_BOOLEAN)
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%s</b>\t%s", count++, consolecmds[i].name,
                    C_LookupAliasFromValue(*(dboolean *)consolecmds[i].variable,
                        consolecmds[i].aliases), description1);
            else if ((consolecmds[i].flags & CF_INTEGER) && (consolecmds[i].flags & CF_PERCENT))
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%i%%</b>\t%s", count++, consolecmds[i].name,
                    *(int *)consolecmds[i].variable, description1);
            else if (consolecmds[i].flags & CF_INTEGER)
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%s</b>\t%s", count++, consolecmds[i].name,
                    C_LookupAliasFromValue(*(int *)consolecmds[i].variable,
                        consolecmds[i].aliases), description1);
            else if (consolecmds[i].flags & CF_FLOAT)
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%s%s</b>\t%s", count++, consolecmds[i].name,
                    striptrailingzero(*(float *)consolecmds[i].variable,
                        ((consolecmds[i].flags & CF_PERCENT) ? 1 : 2)),
                    ((consolecmds[i].flags & CF_PERCENT) ? "%" : ""), description1);
            else if (consolecmds[i].flags & CF_STRING)
                C_TabbedOutput(tabs, "%i.\t<b>%s\t\"%.8s%s\"</b>\t%s", count++,
                    consolecmds[i].name, *(char **)consolecmds[i].variable,
                    (strlen(*(char **)consolecmds[i].variable) > 8 ? "..." : ""), description1);
            else if (consolecmds[i].flags & CF_POSITION)
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%s</b>\t%s", count++, consolecmds[i].name,
                    *(char **)consolecmds[i].variable, description1);
            else if (consolecmds[i].flags & CF_SIZE)
                C_TabbedOutput(tabs, "%i.\t<b>%s\t%s</b>\t%s", count++, consolecmds[i].name,
                    formatsize(*(char **)consolecmds[i].variable), description1);
            else if (consolecmds[i].flags & CF_TIME)
            {
                int     tics = *(int *)consolecmds[i].variable / TICRATE;

                C_TabbedOutput(tabs, "%i.\t<b>%s\t%02i:%02i:%02i</b>\t%s", count++,
                    consolecmds[i].name, tics / 3600, (tics % 3600) / 60, (tics % 3600) % 60,
                    description1);
            }

            if (*description2)
                C_TabbedOutput(tabs, "\t\t\t%s", description2);
        }
        ++i;
    }
}

//
// endgame cmd
//
static void endgame_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    M_EndingGame();
    C_HideConsoleFast();
}

//
// exitmap cmd
//
static void exitmap_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    G_ExitLevel();
    C_HideConsoleFast();
}

//
// fastmonsters cmd
//
void G_SetFastMonsters(dboolean toggle);

static dboolean fastmonsters_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (skilllevel != sk_nightmare);
}

static void fastmonsters_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, 1);

        if (value == 0)
        {
            if (!fastparm)
                return;
            fastparm = false;
        }
        else if (value == 1)
        {
            if (fastparm)
                return;
            fastparm = true;
        }
    }
    else
        fastparm = !fastparm;

    if (fastparm)
    {
        G_SetFastMonsters(true);
        HU_PlayerMessage(s_STSTR_FMON, false);
    }
    else
    {
        G_SetFastMonsters(false);
        HU_PlayerMessage(s_STSTR_FMOFF, false);
    }
}

//
// give cmd
//
extern int      cardsfound;

static dboolean give_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    char        *parm = M_StringJoin(parm1, parm2, parm3, NULL);
    int         i;

    if (gamestate != GS_LEVEL)
        return false;

    if (!*parm)
        return true;

    if (M_StringCompare(parm, "all") || M_StringCompare(parm, "health") || M_StringCompare(parm,
        "weapons") || M_StringCompare(parm, "ammo") || M_StringCompare(parm, "armor")
        || M_StringCompare(parm, "armour") || M_StringCompare(parm, "keys"))
        return true;

    for (i = 0; i < NUMMOBJTYPES; i++)
        if ((mobjinfo[i].flags & MF_SPECIAL) && (M_StringCompare(parm,
            removespaces(mobjinfo[i].name1)) || (*mobjinfo[i].name2 && M_StringCompare(parm,
            removespaces(mobjinfo[i].name2))) || atoi(parm) == mobjinfo[i].doomednum))
            return true;

    return false;
}

static void give_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    char        *parm = M_StringJoin(parm1, parm2, parm3, NULL);

    if (!*parm)
        C_Output("%s %s", cmd, GIVECMDLONGFORMAT);
    else
    {
        player_t    *player = &players[0];

        if (M_StringCompare(parm, "all"))
        {
            P_GiveBackpack(player, false);
            P_GiveMegaHealth(player);
            P_GiveAllWeapons(player);
            P_GiveFullAmmo(player);
            P_GiveArmor(player, blue_armor_class);
            P_GiveAllCards(player);
            C_HideConsole();
        }
        else if (M_StringCompare(parm, "health"))
        {
            P_GiveMegaHealth(player);
            C_HideConsole();
        }
        else if (M_StringCompare(parm, "weapons"))
        {
            P_GiveAllWeapons(player);
            C_HideConsole();
        }
        else if (M_StringCompare(parm, "ammo"))
        {
            P_GiveFullAmmo(player);
            C_HideConsole();
        }
        else if (M_StringCompare(parm, "armor") || M_StringCompare(parm, "armour"))
        {
            P_GiveArmor(player, blue_armor_class);
            C_HideConsole();
        }
        else if (M_StringCompare(parm, "keys"))
        {
            P_GiveAllCards(player);
            C_HideConsole();
        }
        else
        {
            int i;

            for (i = 0; i < NUMMOBJTYPES; i++)
                if ((mobjinfo[i].flags & MF_SPECIAL)
                    && (M_StringCompare(parm, removespaces(mobjinfo[i].name1))
                        || (*mobjinfo[i].name2
                            && M_StringCompare(parm, removespaces(mobjinfo[i].name2)))
                    || atoi(parm) == mobjinfo[i].doomednum))
                {
                    mobj_t *thing = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, i);

                    P_TouchSpecialThing(thing, player->mo, false);
                    C_HideConsole();
                    break;
                }
        }
    }
}

//
// god cmd
//
static dboolean god_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (gamestate == GS_LEVEL && players[0].playerstate == PST_LIVE);
}

static void god_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    player_t    *player = &players[0];

    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, BOOLALIAS);

        if (value == 0)
            player->cheats &= ~CF_GODMODE;
        else if (value == 1)
            player->cheats |= CF_GODMODE;
    }
    else
        player->cheats ^= CF_GODMODE;

    C_Output((player->cheats & CF_GODMODE) ? s_STSTR_GODON : s_STSTR_GODOFF);
}

//
// help cmd
//
static void help_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
#if defined(WIN32)
    ShellExecute(GetActiveWindow(), "open", PACKAGE_WIKI_HELP_URL, NULL, NULL, SW_SHOWNORMAL);
#else
    C_HideConsoleFast();
    M_ShowHelp();
#endif
}

//
// kill cmd
//
static int      killcmdtype = NUMMOBJTYPES;

static dboolean kill_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (gamestate == GS_LEVEL)
    {
        char    *parm = M_StringJoin(parm1, parm2, parm3, NULL);
        int     i;

        if (!*parm)
            return true;

        if (M_StringCompare(parm, "player") || M_StringCompare(parm, "me")
            || (*playername && M_StringCompare(parm, playername)))
            return !!players[0].mo->health;

        if (M_StringCompare(parm, "monsters") || M_StringCompare(parm, "all"))
            return true;

        for (i = 0; i < NUMMOBJTYPES; i++)
        {
            killcmdtype = mobjinfo[i].doomednum;
            if (killcmdtype >= 0 && (M_StringCompare(parm, removespaces(mobjinfo[i].name1))
                || M_StringCompare(parm, removespaces(mobjinfo[i].plural1))
                || (*mobjinfo[i].name2 && M_StringCompare(parm, removespaces(mobjinfo[i].name2)))
                || (*mobjinfo[i].plural2 &&
                    M_StringCompare(parm, removespaces(mobjinfo[i].plural2)))
                || atoi(parm) == killcmdtype))
            {
                dboolean        kill = true;

                if (gamemode != commercial)
                {
                    switch (killcmdtype)
                    {
                        case Arachnotron:
                        case ArchVile:
                        case BossBrain:
                        case HellKnight:
                        case Mancubus:
                        case PainElemental:
                        case HeavyWeaponDude:
                        case Revenant:
                        case WolfensteinSS:
                            kill = false;
                            break;
                    }
                }
                else if (killcmdtype == WolfensteinSS && bfgedition)
                    killcmdtype = Zombieman;

                if (!(mobjinfo[i].flags & MF_SHOOTABLE))
                    kill = false;

                return kill;
            }
        }
    }
    return false;
}

void A_Fall(mobj_t *actor, player_t *player, pspdef_t *psp);

static void kill_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    char        *parm = M_StringJoin(parm1, parm2, parm3, NULL);
    static char buffer[1024];

    if (!*parm)
        C_Output("%s %s", cmd, KILLCMDFORMAT);
    else if (M_StringCompare(parm, "player") || M_StringCompare(parm, "me")
        || (*playername && M_StringCompare(parm, playername)))
    {
        players[0].health = 0;
        P_KillMobj(players[0].mo, players[0].mo);
        M_snprintf(buffer, sizeof(buffer), "%s killed %s", playername,
            (M_StringCompare(playername, "you") ? "yourself" : "themselves"));
        buffer[0] = toupper(buffer[0]);
        C_Output("%s.", buffer);
        players[0].message = buffer;
        message_dontfuckwithme = true;
        C_HideConsole();
    }
    else
    {
        int     i;
        int     kills = 0;

        if (M_StringCompare(parm, "all") || M_StringCompare(parm, "monsters"))
        {
            for (i = 0; i < numsectors; ++i)
            {
                mobj_t      *thing = sectors[i].thinglist;

                while (thing)
                {
                    if (thing->health > 0)
                    {
                        mobjtype_t      type = thing->type;

                        if (type == MT_PAIN)
                        {
                            A_Fall(thing, NULL, NULL);
                            P_SetMobjState(thing, S_PAIN_DIE6);
                            players[0].killcount++;
                            stat_monsterskilled = SafeAdd(stat_monsterskilled, 1);
                            kills++;
                        }
                        else if ((thing->flags & MF_SHOOTABLE) && type != MT_PLAYER
                            && type != MT_BARREL && type != MT_BOSSBRAIN)
                        {
                            thing->flags2 |= MF2_MASSACRE;
                            P_DamageMobj(thing, NULL, NULL, thing->health);
                            if (!(thing->flags & MF_NOBLOOD))
                            {
                                int r;

                                thing->momx += FRACUNIT * (r = M_RandomInt(-1, 1));
                                thing->momy += FRACUNIT * M_RandomIntNoRepeat(-1, 1, (!r ? 0 : 2));
                            }
                            kills++;
                        }
                    }
                    thing = thing->snext;
                }
            }

            if (kills)
            {
                M_snprintf(buffer, sizeof(buffer), "%s monster%s killed", commify(kills),
                    (kills == 1 ? "" : "s"));
                C_Output("%s.", buffer);
                players[0].message = buffer;
                message_dontfuckwithme = true;
                C_HideConsole();
            }
            else
                C_Output("No monsters %s kill.", (!totalkills ? "to" : "left to"));
        }
        else
        {
            mobjtype_t  type = P_FindDoomedNum(killcmdtype);
            int         dead = 0;

            for (i = 0; i < numsectors; ++i)
            {
                mobj_t      *thing = sectors[i].thinglist;

                while (thing)
                {
                    if (type == thing->type)
                        if (type == MT_PAIN)
                        {
                            if (thing->health > 0)
                            {
                                A_Fall(thing, NULL, NULL);
                                P_SetMobjState(thing, S_PAIN_DIE6);
                                players[0].killcount++;
                                stat_monsterskilled = SafeAdd(stat_monsterskilled, 1);
                                kills++;
                            }
                            else
                                dead++;
                        }
                        else if ((thing->flags & MF_SHOOTABLE) && thing->health > 0)
                        {
                            thing->flags2 |= MF2_MASSACRE;
                            P_DamageMobj(thing, NULL, NULL, thing->health);
                            if (!(thing->flags & MF_NOBLOOD))
                            {
                                int r;

                                thing->momx += FRACUNIT * (r = M_RandomInt(-1, 1));
                                thing->momy += FRACUNIT * M_RandomIntNoRepeat(-1, 1, (!r ? 0 : 2));
                            }
                            kills++;
                        }
                        else if (thing->flags & MF_CORPSE)
                            dead++;
                        thing = thing->snext;
                }
            }

            if (kills)
            {
                M_snprintf(buffer, sizeof(buffer), "%s %s %s", commify(kills),
                    (kills == 1 ? mobjinfo[type].name1 : mobjinfo[type].plural1),
                    (type == MT_BARREL ? "exploded" : "killed"));
                C_Output("%s.", buffer);
                players[0].message = buffer;
                message_dontfuckwithme = true;
                C_HideConsole();
            }
            else
                C_Output("No %s %s %s.", mobjinfo[type].plural1, (dead ? "left to" : "to"),
                    (type == MT_BARREL ? "explode" : "kill"));
        }
    }
}

//
// load cmd
//
static dboolean load_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return *parm1;
}

static void load_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    G_LoadGame(M_StringJoin(savegamefolder, parm1,
        (M_StringEndsWith(parm1, ".save") ? "" : ".save"), NULL));
}

//
// map cmd
//
static int      mapcmdepisode;
static int      mapcmdmap;
static char     mapcmdlump[7];

extern dboolean samelevel;
extern menu_t   EpiDef;
extern int      idclevtics;

static dboolean map_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (!*parm1)
        return true;

    mapcmdepisode = 0;
    mapcmdmap = 0;
    parm1 = uppercase(parm1);

    if (M_StringCompare(parm1, "FIRST"))
    {
        if (gamestate != GS_LEVEL)
            return false;
        if (gamemode == commercial)
        {
            if (gamemap == 1)
                return false;
            mapcmdepisode = gameepisode;
            mapcmdmap = 1;
            M_StringCopy(mapcmdlump, "MAP01", 6);
        }
        else
        {
            if (gameepisode == 1 && gamemap == 1)
                return false;
            mapcmdepisode = 1;
            mapcmdmap = 1;
            M_StringCopy(mapcmdlump, "E1M1", 5);
        }
        return true;
    }
    else if (M_StringCompare(parm1, "PREVIOUS") || M_StringCompare(parm1, "PREV"))
    {
        if (gamestate != GS_LEVEL)
            return false;
        if (gamemode == commercial)
        {
            if (gamemap == 1)
                return false;
            mapcmdepisode = gameepisode;
            mapcmdmap = gamemap - 1;
            M_snprintf(mapcmdlump, 6, "MAP%02i", mapcmdmap);
        }
        else
        {
            if (gamemap == 1)
            {
                if (gameepisode == 1)
                    return false;
                else
                {
                    mapcmdepisode = gameepisode - 1;
                    mapcmdmap = 8;
                }
            }
            else
            {
                mapcmdepisode = gameepisode;
                mapcmdmap = gamemap - 1;
            }
            M_snprintf(mapcmdlump, 5, "E%iM%i", mapcmdepisode, mapcmdmap);
        }
        return true;
    }
    else if (M_StringCompare(parm1, "NEXT"))
    {
        if (gamestate != GS_LEVEL)
            return false;
        if (gamemode == commercial)
        {
            if (gamemap == (gamemission == pack_nerve ? 8 : 30))
                return false;
            mapcmdepisode = gameepisode;
            mapcmdmap = gamemap + 1;
            M_snprintf(mapcmdlump, 6, "MAP%02i", mapcmdmap);
        }
        else
        {
            if (gamemap == 8)
            {
                if (gameepisode == (gamemode == retail ? 4 : gamemode == shareware ? 1 : 3))
                    return false;
                else
                {
                    mapcmdepisode = gameepisode + 1;
                    mapcmdmap = 1;
                }
            }
            else
            {
                mapcmdepisode = gameepisode;
                mapcmdmap = gamemap + 1;
            }
            M_snprintf(mapcmdlump, 5, "E%iM%i", mapcmdepisode, mapcmdmap);
        }
        return true;
    }
    else if (M_StringCompare(parm1, "LAST"))
    {
        if (gamestate != GS_LEVEL)
            return false;
        if (gamemode == commercial)
        {
            if (gamemission == pack_nerve)
            {
                if (gamemap == 8)
                    return false;
                mapcmdepisode = gameepisode;
                mapcmdmap = 8;
                M_StringCopy(mapcmdlump, "MAP08", 6);
            }
            else
            {
                if (gamemap == 30)
                    return false;
                mapcmdepisode = gameepisode;
                mapcmdmap = 30;
                M_StringCopy(mapcmdlump, "MAP30", 6);
            }
        }
        else if (gamemode == retail)
        {
            if (gameepisode == 4 && gamemap == 8)
                return false;
            mapcmdepisode = 4;
            mapcmdmap = 8;
            M_StringCopy(mapcmdlump, "E4M8", 5);
        }
        else if (gamemode == shareware)
        {
            if (gameepisode == 1 && gamemap == 8)
                return false;
            mapcmdepisode = 1;
            mapcmdmap = 8;
            M_StringCopy(mapcmdlump, "E1M8", 5);
        }
        else
        {
            if (gameepisode == 4 && gamemap == 8)
                return false;
            mapcmdepisode = 3;
            mapcmdmap = 8;
            M_StringCopy(mapcmdlump, "E3M8", 5);
        }
        return true;
    }
    M_StringCopy(mapcmdlump, parm1, 7);
    if (gamemode == commercial)
    {
        mapcmdepisode = 1;

        if (sscanf(parm1, "MAP0%1i", &mapcmdmap) == 1 || sscanf(parm1, "MAP%2i", &mapcmdmap) == 1)
            if ((BTSX && W_CheckMultipleLumps(parm1) == 1)
                || (gamemission == pack_nerve && mapcmdmap > 9))
                return false;
            else
            {
                if (gamestate != GS_LEVEL && gamemission == pack_nerve)
                {
                    gamemission = doom2;
                    expansion = 0;
                }
                return (W_CheckNumForName(parm1) >= 0);
            }

        if (BTSX)
        {
            if (sscanf(parm1, "MAP%02iC", &mapcmdmap) == 1)
                return (W_CheckNumForName(parm1) >= 0);
            else
            {
                if (sscanf(parm1, "E%1iM0%1i", &mapcmdepisode, &mapcmdmap) != 2)
                    sscanf(parm1, "E%1iM%2i", &mapcmdepisode, &mapcmdmap);
                if (mapcmdmap && ((mapcmdepisode == 1 && BTSXE1) || (mapcmdepisode == 2 && BTSXE2)
                    || (mapcmdepisode == 3 && BTSXE3)))
                {
                    static char     lump[6];

                    M_snprintf(lump, sizeof(lump), "MAP%02i", mapcmdmap);
                    return (W_CheckMultipleLumps(lump) == 2);
                }
            }
        }
    }
    else if (sscanf(parm1, "E%1iM%1i", &mapcmdepisode, &mapcmdmap) == 2)
    {
        episode = mapcmdepisode - 1;

        return (W_CheckNumForName(parm1) >= 0);
    }
    else if (FREEDOOM && sscanf(parm1, "C%1iM%1i", &mapcmdepisode, &mapcmdmap) == 2)
    {
        static char     lump[5];

        M_snprintf(lump, sizeof(lump), "E%iM%i", mapcmdepisode, mapcmdmap);
        return (W_CheckNumForName(lump) >= 0);
    }

    return false;
}

static void map_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    static char buffer[1024];

    if (!*parm1)
    {
        C_Output("%s %s", cmd, MAPCMDLONGFORMAT);
        return;
    }
    samelevel = (gameepisode == mapcmdepisode && gamemap == mapcmdmap);
    gameepisode = mapcmdepisode;
    if (gamemission == doom && gameepisode <= 4)
    {
        episode = gameepisode - 1;
        EpiDef.lastOn = episode;
    }
    gamemap = mapcmdmap;
    M_snprintf(buffer, sizeof(buffer), (samelevel ? "Restarting %s..." : "Warping to %s..."),
        uppercase(mapcmdlump));
    C_Output(buffer);
    players[0].message = buffer;
    message_dontfuckwithme = true;
    if (gamestate == GS_LEVEL)
    {
        idclevtics = MAPCHANGETICS;
        C_HideConsole();
    }
    else
    {
        G_DeferredInitNew(skilllevel, gameepisode, gamemap);
        C_HideConsoleFast();
    }
}

//
// maplist cmd
//
extern int      dehcount;
extern char     **mapnames[];
extern char     **mapnames2[];
extern char     **mapnames2_bfg[];
extern char     **mapnamesp[];
extern char     **mapnamest[];
extern char     **mapnamesn[];

static void maplist_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int         i, j;
    int         count = 0;
    int         tabs[8] = { 40, 90, 350, 0, 0, 0, 0, 0 };
    char        (*maplist)[256] = malloc(numlumps * sizeof(char *));

    // search through lumps for maps
    for (i = 0; i < numlumps; ++i)
    {
        int             ep = 0;
        int             map = 0;
        char            lump[8];
        char            wad[MAX_PATH];
        dboolean        replaced;
        dboolean        pwad;
        char            mapinfoname[128];

        M_StringCopy(lump, uppercase(lumpinfo[i]->name), 8);

        if (gamemode == commercial)
        {
            ep = 1;
            sscanf(lump, "MAP0%1i", &map);
            if (!map)
                sscanf(lump, "MAP%2i", &map);
        }
        else
            sscanf(lump, "E%1iM%1i", &ep, &map);

        if (!ep-- || !map--)
            continue;

        M_StringCopy(wad, uppercase(leafname(lumpinfo[i]->wad_file->path)), MAX_PATH);
        replaced = (W_CheckMultipleLumps(lump) > 1 && !chex && !FREEDOOM);
        pwad = (lumpinfo[i]->wad_file->type == PWAD);
        M_StringCopy(mapinfoname, P_GetMapName(ep * 10 + map + 1), 128);

        switch (gamemission)
        {
            case doom:
                if (!replaced || pwad)
                    M_snprintf(maplist[count++], 256, "%s\t%s\t%s", lump, (replaced
                        && dehcount == 1 && !*mapinfoname ? "-" : titlecase(*mapinfoname ?
                        mapinfoname : *mapnames[ep * 9 + map])), (modifiedgame ? wad : ""));
                break;

            case doom2:
                if (!M_StringCompare(wad, "nerve.wad") && (!replaced || pwad || nerve)
                    && (pwad || !BTSX))
                    if (BTSX)
                    {
                        if (strchr(*mapnames2[map], ':'))
                            M_snprintf(maplist[count++], 256, "%s",
                                M_StringReplace(*mapnames2[map], ": ", "\t"));
                    }
                    else
                        M_snprintf(maplist[count++], 256, "%s\t%s\t%s", lump, (replaced
                            && dehcount == 1 && !nerve && !*mapinfoname ? "-" :
                            titlecase(*mapinfoname ? mapinfoname : (bfgedition ?
                            *mapnames2_bfg[map] : *mapnames2[map]))), (modifiedgame && !nerve ?
                            wad : ""));
                break;

            case pack_nerve:
                if (M_StringCompare(wad, "nerve.wad"))
                    M_snprintf(maplist[count++], 256, "%s\t%s", lump, titlecase(*mapinfoname ?
                        mapinfoname : *mapnamesn[map]));
                break;

            case pack_plut:
                if (!replaced || pwad)
                    M_snprintf(maplist[count++], 256, "%s\t%s\t%s", lump, (replaced
                        && dehcount == 1 && !*mapinfoname ? "-" : titlecase(*mapinfoname ?
                        mapinfoname : *mapnamesp[map])), (modifiedgame ? wad : ""));
                break;

            case pack_tnt:
                if (!replaced || pwad)
                    M_snprintf(maplist[count++], 256, "%s\t%s\t%s", lump, (replaced
                        && dehcount == 1 && !*mapinfoname ? "-" : titlecase(*mapinfoname ?
                        mapinfoname : *mapnamest[map])), (modifiedgame ? wad : ""));
                break;
        }
    }

    // sort the map list
    for (i = 0; i < count; i++)
        for (j = i + 1; j < count; j++)
        {
            if (strcmp(maplist[i], maplist[j]) > 0)
            {
                char    temp[256];

                strcpy(temp, maplist[i]);
                strcpy(maplist[i], maplist[j]);
                strcpy(maplist[j], temp);
            }
        }

    // display the map list
    for (i = 0; i < count; ++i)
        C_TabbedOutput(tabs, "%i.\t%s", i + 1, maplist[i]);

    free(maplist);
}

//
// mapstats cmd
//
#define AA      "Andre Arsenault"
#define AD      "Andrew Dowswell"
#define AM      "American McGee"
#define BK      "Brian Kidby"
#define CB      "Christopher Buteau"
#define DB      "David Blanshine"
#define DC      "Dario Casali"
#define DC2     "David Calvin"
#define DJ      "Dean Johnson"
#define DO      "Drake O'Brien"
#define JA      "John Anderson"
#define JD      "Jim Dethlefsen"
#define JL      "Jim Lowell"
#define JM      "Jim Mentzer"
#define JM2     "John Minadeo"
#define JR      "John Romero"
#define JS      "Jimmy Sieben"
#define JW      "John Wakelin"
#define MB      "Michael Bukowski"
#define MC      "Milo Casali"
#define MS      "Mark Snell"
#define PT      "Paul Turnbull"
#define RH      "Richard Heath"
#define RM      "Russell Meakim"
#define RP      "Robin Patenall"
#define SG      "Shawn Green"
#define SP      "Sandy Petersen"
#define TH      "Tom Hall"
#define TH2     "Ty Halderman"
#define TM      "Tom Mustaine"
#define TW      "Tim Willits"
#define WW      "William D. Whitaker"
#define AMSP    AM" and "SP
#define BKTH2   BK" and "TH2
#define DC2DB   DC2", "DB
#define DCMC    DC" and "MC
#define DCTH2   DC" and "TH2
#define JRTH    JR" and "TH
#define JSTH2   JS" and "TH2
#define MSJL    MS" and "JL
#define RPJM2   RP" and "JM2
#define SPTH    SP" and "TH

char *authors[][6] =
{
    /* xy      doom   doom2  tnt    plut   nerve */
    /* 00 */ { "",    "",    "",    DCMC,  "" },
    /* 01 */ { "",    SP,    TM,    DCMC,  RM },
    /* 02 */ { "",    AM,    JW,    DCMC,  RH },
    /* 03 */ { "",    AM,    RPJM2, DCMC,  RM },
    /* 04 */ { "",    AM,    TH2,   DCMC,  RM },
    /* 05 */ { "",    AM,    JD,    DCMC,  RH },
    /* 06 */ { "",    AM,    JSTH2, DCMC,  RH },
    /* 07 */ { "",    AMSP,  AD,    DCMC,  RH },
    /* 08 */ { "",    SP,    JM2,   DCMC,  RH },
    /* 09 */ { "",    SP,    JSTH2, DCMC,  RM },
    /* 10 */ { "",    SPTH,  TM,    DCMC,  "" },
    /* 11 */ { JR,    JR,    DJ,    DCMC,  "" },
    /* 12 */ { JR,    SP,    JL,    DCMC,  "" },
    /* 13 */ { JR,    SP,    BKTH2, DCMC,  "" },
    /* 14 */ { JRTH,  AM,    RP,    DCMC,  "" },
    /* 15 */ { JR,    JR,    WW,    DCMC,  "" },
    /* 16 */ { JR,    SP,    AA,    DCMC,  "" },
    /* 17 */ { JR,    JR,    TM,    DCMC,  "" },
    /* 18 */ { SPTH,  SP,    DCTH2, DCMC,  "" },
    /* 19 */ { JR,    SP,    TH2,   DCMC,  "" },
    /* 20 */ { DC2DB, JR,    DO,    DCMC,  "" },
    /* 21 */ { SPTH,  SP,    DO,    DCMC,  "" },
    /* 22 */ { SPTH,  AM,    CB,    DCMC,  "" },
    /* 23 */ { SPTH,  SP,    PT,    DCMC,  "" },
    /* 24 */ { SPTH,  SP,    DJ,    DCMC,  "" },
    /* 25 */ { SP,    SG,    JM,    DCMC,  "" },
    /* 26 */ { SP,    JR,    MSJL,  DCMC,  "" },
    /* 27 */ { SPTH,  SP,    DO,    DCMC,  "" },
    /* 28 */ { SP,    SP,    MC,    DCMC,  "" },
    /* 29 */ { SP,    JR,    JS,    DCMC,  "" },
    /* 30 */ { "",    SP,    JS,    DCMC,  "" },
    /* 31 */ { SP,    SP,    DC,    DCMC,  "" },
    /* 32 */ { SP,    SP,    DC,    DCMC,  "" },
    /* 33 */ { SPTH,  MB,    "",    "",    "" },
    /* 34 */ { SP,    "",    "",    "",    "" },
    /* 35 */ { SP,    "",    "",    "",    "" },
    /* 36 */ { SP,    "",    "",    "",    "" },
    /* 37 */ { SPTH,  "",    "",    "",    "" },
    /* 38 */ { SP,    "",    "",    "",    "" },
    /* 39 */ { SP,    "",    "",    "",    "" },
    /* 40 */ { "",    "",    "",    "",    "" },
    /* 41 */ { AM,    "",    "",    "",    "" },
    /* 42 */ { JR,    "",    "",    "",    "" },
    /* 43 */ { SG,    "",    "",    "",    "" },
    /* 44 */ { AM,    "",    "",    "",    "" },
    /* 45 */ { TW,    "",    "",    "",    "" },
    /* 46 */ { JR,    "",    "",    "",    "" },
    /* 47 */ { JA,    "",    "",    "",    "" },
    /* 48 */ { SG,    "",    "",    "",    "" },
    /* 49 */ { TW,    "",    "",    "",    "" }
};

static void mapstats_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int tabs[8] = { 120, 240, 0, 0, 0, 0, 0, 0 };

    C_TabbedOutput(tabs, "Title\t%s", mapnumandtitle);

    {
        int     i = (gamemission == doom ? gameepisode * 10 : 0) + gamemap;
        char    *author = P_GetMapAuthor(i);

        if (*author)
            C_TabbedOutput(tabs, "Author\t%s", author);
        else if (canmodify && *authors[i][gamemission])
            C_TabbedOutput(tabs, "Author\t%s", authors[i][gamemission]);
    }

    {
        static char     lumpname[6];
        int             i;

        if (gamemode == commercial)
            M_snprintf(lumpname, sizeof(lumpname), "MAP%02i", startmap);
        else
            M_snprintf(lumpname, sizeof(lumpname), "E%iM%i", startepisode, startmap);
        i = (nerve && gamemission == doom2 ? W_GetNumForName2(lumpname) :
            W_CheckNumForName(lumpname));
        C_TabbedOutput(tabs, "%s\t%s", (lumpinfo[i]->wad_file->type == IWAD ? "IWAD" : "PWAD"),
            uppercase(leafname(lumpinfo[i]->wad_file->path)));
    }

    C_TabbedOutput(tabs, "Things\t%s\t%s", commify(numthings), convertsize(sizethings));

    C_TabbedOutput(tabs, "Lines\t%s\t%s", commify(numlines), convertsize(sizelines));

    C_TabbedOutput(tabs, "Line specials\t%s-compatible",
        (boomlinespecials ? "<i>BOOM</i>" : "Vanilla"));

    C_TabbedOutput(tabs, "Sides\t%s\t%s", commify(numsides), convertsize(sizesides));

    C_TabbedOutput(tabs, "Vertices\t%s\t%s", commify(numvertexes), convertsize(sizevertexes));

    C_TabbedOutput(tabs, "Segments\t%s\t%s", commify(numsegs), convertsize(sizesegs));

    C_TabbedOutput(tabs, "Subsectors\t%s\t%s", commify(numsubsectors),
        convertsize(sizesubsectors));

    C_TabbedOutput(tabs, "Nodes\t%s\t%s", commify(numnodes), convertsize(sizenodes));

    C_TabbedOutput(tabs, "Node format\t%s", (mapformat == DOOMBSP ? "Regular nodes" :
        (mapformat == DEEPBSP ? "DeePBSP v4 extended nodes" :
            "ZDoom uncompressed extended nodes")));

    C_TabbedOutput(tabs, "Sectors\t%s\t%s", commify(numsectors), convertsize(sizesectors));

    if (blockmaprecreated)
        C_TabbedOutput(tabs, "Blockmap\tRecreated");

    {
        int     i;
        int     min_x = INT_MAX;
        int     max_x = INT_MIN;
        int     min_y = INT_MAX;
        int     max_y = INT_MIN;

        for (i = 0; i < numvertexes; ++i)
        {
            fixed_t     x = vertexes[i].x;
            fixed_t     y = vertexes[i].y;

            if (x < min_x)
                min_x = x;
            else if (x > max_x)
                max_x = x;
            if (y < min_y)
                min_y = y;
            else if (y > max_y)
                max_y = y;
        }
        C_TabbedOutput(tabs, "Dimensions\t%s\xD7%s",
            commify((max_x - min_x) >> FRACBITS), commify((max_y - min_y) >> FRACBITS));
    }

    if (mus_playing && !nomusic)
    {
        int     lumps = W_CheckMultipleLumps(mus_playing->name);

        if (((gamemode == commercial || gameepisode > 1) && lumps == 1)
            || (gamemode != commercial && gameepisode == 1 && lumps == 2))
            C_TabbedOutput(tabs, "Music title\t%s", mus_playing->title);
    }
}

//
// noclip cmd
//
static void noclip_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    player_t    *player = &players[0];

    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, 1);

        if (value == 0)
            player->cheats &= ~CF_NOCLIP;
        else if (value == 1)
            player->cheats |= CF_NOCLIP;
    }
    else
        player->cheats ^= CF_NOCLIP;

    HU_PlayerMessage(((player->cheats & CF_NOCLIP) ? s_STSTR_NCON : s_STSTR_NCOFF), false);
}

//
// nomonsters cmd
//
static void nomonsters_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, 1);

        if (value == 0)
            nomonsters = false;
        else if (value == 1)
            nomonsters = true;
    }
    else
        nomonsters = !nomonsters;

    HU_PlayerMessage((nomonsters ? s_STSTR_NMON : s_STSTR_NMOFF), false);
}

//
// notarget cmd
//
static void notarget_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    player_t    *player = &players[0];

    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, 1);

        if (value == 0)
            player->cheats &= ~CF_NOTARGET;
        else if (value == 1)
            player->cheats |= CF_NOTARGET;
    }
    else
        player->cheats ^= CF_NOTARGET;

    if (player->cheats & CF_NOTARGET)
    {
        int     i;

        for (i = 0; i < numsectors; ++i)
        {
            mobj_t   *mo = sectors[i].thinglist;

            while (mo)
            {
                if (mo->target && mo->target->player)
                    P_SetTarget(&mo->target, NULL);
                if (mo->tracer && mo->tracer->player)
                    P_SetTarget(&mo->tracer, NULL);
                if (mo->lastenemy && mo->lastenemy->player)
                    P_SetTarget(&mo->lastenemy, NULL);
                mo = mo->snext;
            }

            P_SetTarget(&sectors[i].soundtarget, NULL);
        }
        HU_PlayerMessage(s_STSTR_NTON, false);
    }
    else
        HU_PlayerMessage(s_STSTR_NTOFF, false);
}

//
// pistolstart cmd
//
static void pistolstart_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, 1);

        if (value == 0)
            pistolstart = false;
        else if (value == 1)
            pistolstart = true;
    }
    else
        pistolstart = !pistolstart;

    HU_PlayerMessage((pistolstart ? s_STSTR_PSON : s_STSTR_PSOFF), false);
}

static void C_PlayerStats_Game(void)
{
    int         tabs[8] = { 160, 270, 0, 0, 0, 0, 0, 0 };
    int         time1 = leveltime / TICRATE;
    int         time2 = stat_time / TICRATE;
    player_t    *player = &players[0];

    C_TabbedOutput(tabs, "\t<i>Current Map</i>\t<i>Total</i>");

    if ((players[0].cheats & CF_ALLMAP) || (players[0].cheats & CF_ALLMAP_THINGS))
        C_TabbedOutput(tabs, "Map revealed\t100%%\t-");
    else
    {
        int     i = 0;
        int     totallines = numlines;
        int     totallinesmapped = 0;

        while (i < numlines)
            totallines -= !!(lines[i++].flags & ML_DONTDRAW);
        i = 0;
        while (i < totallines)
            totallinesmapped += !!(lines[i++].flags & ML_MAPPED);
        C_TabbedOutput(tabs, "Map revealed\t%s%%\t-",
            striptrailingzero(totallinesmapped * 100.0f / totallines, 1));
    }

    C_TabbedOutput(tabs, "Monsters killed\t%s of %s (%i%%)\t%s",
        commify(player->killcount), commify(totalkills),
        (totalkills ? player->killcount * 100 / totalkills : 0), commify(stat_monsterskilled));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_BABY].plural1),
            commify(player->mobjcount[MT_BABY]), commify(monstercount[MT_BABY]),
            (monstercount[MT_BABY] ? player->mobjcount[MT_BABY] * 100 / monstercount[MT_BABY] : 0),
            commify(stat_monsterskilled_arachnotrons));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_VILE].plural1),
            commify(player->mobjcount[MT_VILE]), commify(monstercount[MT_VILE]),
            (monstercount[MT_VILE] ? player->mobjcount[MT_VILE] * 100 / monstercount[MT_VILE] : 0),
            commify(stat_monsterskilled_archviles));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_BRUISER].plural1),
        commify(player->mobjcount[MT_BRUISER]), commify(monstercount[MT_BRUISER]),
        (monstercount[MT_BRUISER] ? player->mobjcount[MT_BRUISER] * 100 / monstercount[MT_BRUISER]
        : 0), commify(stat_monsterskilled_baronsofhell));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_HEAD].plural1),
        commify(player->mobjcount[MT_HEAD]), commify(monstercount[MT_HEAD]),
        (monstercount[MT_HEAD] ? player->mobjcount[MT_HEAD] * 100 / monstercount[MT_HEAD] : 0),
        commify(stat_monsterskilled_cacodemons));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_CYBORG].plural1),
        commify(player->mobjcount[MT_CYBORG]), commify(monstercount[MT_CYBORG]),
        (monstercount[MT_CYBORG] ? player->mobjcount[MT_CYBORG] * 100 / monstercount[MT_CYBORG]
        : 0), commify(stat_monsterskilled_cyberdemons));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_SERGEANT].plural1),
        commify(player->mobjcount[MT_SERGEANT]), commify(monstercount[MT_SERGEANT]),
        (monstercount[MT_SERGEANT] ? player->mobjcount[MT_SERGEANT] * 100
        / monstercount[MT_SERGEANT] : 0), commify(stat_monsterskilled_demons));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_CHAINGUY].plural1),
            commify(player->mobjcount[MT_CHAINGUY]), commify(monstercount[MT_CHAINGUY]),
            (monstercount[MT_CHAINGUY] ? player->mobjcount[MT_CHAINGUY] * 100
            / monstercount[MT_CHAINGUY] : 0), commify(stat_monsterskilled_heavyweapondudes));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_KNIGHT].plural1),
            commify(player->mobjcount[MT_KNIGHT]), commify(monstercount[MT_KNIGHT]),
            (monstercount[MT_KNIGHT] ? player->mobjcount[MT_KNIGHT] * 100 / monstercount[MT_KNIGHT]
            : 0), commify(stat_monsterskilled_hellknights));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_TROOP].plural1),
        commify(player->mobjcount[MT_TROOP]), commify(monstercount[MT_TROOP]),
        (monstercount[MT_TROOP] ? player->mobjcount[MT_TROOP] * 100 / monstercount[MT_TROOP] : 0),
        commify(stat_monsterskilled_imps));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_SKULL].plural1),
        commify(player->mobjcount[MT_SKULL]), commify(monstercount[MT_SKULL]),
        (monstercount[MT_SKULL] ? player->mobjcount[MT_SKULL] * 100 / monstercount[MT_SKULL] : 0),
        commify(stat_monsterskilled_lostsouls));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_FATSO].plural1),
            commify(player->mobjcount[MT_FATSO]), commify(monstercount[MT_FATSO]),
            (monstercount[MT_FATSO] ? player->mobjcount[MT_FATSO] * 100 / monstercount[MT_FATSO]
            : 0), commify(stat_monsterskilled_mancubi));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_PAIN].plural1),
            commify(player->mobjcount[MT_PAIN]), commify(monstercount[MT_PAIN]),
            (monstercount[MT_PAIN] ? player->mobjcount[MT_PAIN] * 100 / monstercount[MT_PAIN] : 0),
            commify(stat_monsterskilled_painelementals));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_UNDEAD].plural1),
        commify(player->mobjcount[MT_UNDEAD]), commify(monstercount[MT_UNDEAD]),
        (monstercount[MT_UNDEAD] ? player->mobjcount[MT_UNDEAD] * 100 / monstercount[MT_UNDEAD]
        : 0), commify(stat_monsterskilled_revenants));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_SHOTGUY].plural1),
        commify(player->mobjcount[MT_SHOTGUY]), commify(monstercount[MT_SHOTGUY]),
        (monstercount[MT_SHOTGUY] ? player->mobjcount[MT_SHOTGUY] * 100 / monstercount[MT_SHOTGUY]
        : 0), commify(stat_monsterskilled_shotgunguys));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_SHADOWS].plural1),
        commify(player->mobjcount[MT_SHADOWS]), commify(monstercount[MT_SHADOWS]),
        (monstercount[MT_SHADOWS] ? player->mobjcount[MT_SHADOWS] * 100 / monstercount[MT_SHADOWS]
        : 0), commify(stat_monsterskilled_spectres));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_SPIDER].plural1),
        commify(player->mobjcount[MT_SPIDER]), commify(monstercount[MT_SPIDER]),
        (monstercount[MT_SPIDER] ? player->mobjcount[MT_SPIDER] * 100 / monstercount[MT_SPIDER]
        : 0), commify(stat_monsterskilled_spidermasterminds));

    C_TabbedOutput(tabs, "   %s\t%s of %s (%i%%)\t%s", titlecase(mobjinfo[MT_POSSESSED].plural1),
        commify(player->mobjcount[MT_POSSESSED]), commify(monstercount[MT_POSSESSED]),
        (monstercount[MT_POSSESSED] ? player->mobjcount[MT_POSSESSED] * 100
        / monstercount[MT_POSSESSED] : 0), commify(stat_monsterskilled_zombiemen));

    C_TabbedOutput(tabs, "Items picked up\t%s of %s (%i%%)\t%s",
        commify(player->itemcount), commify(totalitems),
        (totalitems ? player->itemcount * 100 / totalitems : 0), commify(stat_itemspickedup));

    C_TabbedOutput(tabs, "Secrets revealed\t%s of %s (%i%%)\t%s",
        commify(player->secretcount), commify(totalsecret),
        (totalsecret ? player->secretcount * 100 / totalsecret : 0),
        commify(stat_secretsrevealed));

    C_TabbedOutput(tabs, "Time played\t%02i:%02i:%02i\t%02i:%02i:%02i",
        time1 / 3600, (time1 % 3600) / 60, (time1 % 3600) % 60,
        time2 / 3600, (time2 % 3600) / 60, (time2 % 3600) % 60);

    C_TabbedOutput(tabs, "Damage inflicted\t%s\t%s",
        commify(player->damageinflicted), commify(stat_damageinflicted));

    C_TabbedOutput(tabs, "Damage received\t%s\t%s",
        commify(player->damagereceived), commify(stat_damagereceived));

    C_TabbedOutput(tabs, "Deaths\t%s\t%s", commify(player->deaths), commify(stat_deaths));

    C_TabbedOutput(tabs, "Cheated\t%s\t%s",
        commify(player->cheated), commify(stat_cheated));

    C_TabbedOutput(tabs, "Shots fired\t%s\t%s",
        commify(player->shotsfired), commify(stat_shotsfired));

    C_TabbedOutput(tabs, "Shots hit\t%s\t%s",
        commify(player->shotshit), commify(stat_shotshit));

    C_TabbedOutput(tabs, "Weapon accuracy\t%s%%\t%s%%",
        (player->shotsfired ? striptrailingzero(player->shotshit * 100.0f / player->shotsfired,
        1) : "0"), (stat_shotsfired ? striptrailingzero(stat_shotshit * 100.0f / stat_shotsfired,
        1) : "0"));
}

static void C_PlayerStats_NoGame(void)
{
    int tabs[8] = { 160, 0, 0, 0, 0, 0, 0, 0 };
    int time2 = stat_time / TICRATE;

    C_TabbedOutput(tabs, "\t<i>Total</i>");

    C_TabbedOutput(tabs, "Monsters killed\t%s", commify(stat_monsterskilled));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_BABY].plural1),
            commify(stat_monsterskilled_arachnotrons));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_VILE].plural1),
            commify(stat_monsterskilled_archviles));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_BRUISER].plural1),
        commify(stat_monsterskilled_baronsofhell));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_HEAD].plural1),
        commify(stat_monsterskilled_cacodemons));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_CYBORG].plural1),
        commify(stat_monsterskilled_cyberdemons));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_SERGEANT].plural1),
        commify(stat_monsterskilled_demons));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_CHAINGUY].plural1),
            commify(stat_monsterskilled_heavyweapondudes));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_KNIGHT].plural1),
            commify(stat_monsterskilled_hellknights));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_TROOP].plural1),
        commify(stat_monsterskilled_imps));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_SKULL].plural1),
        commify(stat_monsterskilled_lostsouls));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_FATSO].plural1),
            commify(stat_monsterskilled_mancubi));

    if (gamemode == commercial)
        C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_PAIN].plural1),
            commify(stat_monsterskilled_painelementals));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_UNDEAD].plural1),
        commify(stat_monsterskilled_revenants));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_SHOTGUY].plural1),
        commify(stat_monsterskilled_shotgunguys));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_SHADOWS].plural1),
        commify(stat_monsterskilled_spectres));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_SPIDER].plural1),
        commify(stat_monsterskilled_spidermasterminds));

    C_TabbedOutput(tabs, "   %s\t%s", titlecase(mobjinfo[MT_POSSESSED].plural1),
        commify(stat_monsterskilled_zombiemen));

    C_TabbedOutput(tabs, "Items picked up\t%s", commify(stat_itemspickedup));

    C_TabbedOutput(tabs, "Secrets revealed\t%s", commify(stat_secretsrevealed));

    C_TabbedOutput(tabs, "Time played\t%02i:%02i:%02i",
        time2 / 3600, (time2 % 3600) / 60, (time2 % 3600) % 60);

    C_TabbedOutput(tabs, "Damage inflicted\t%s", commify(stat_damageinflicted));

    C_TabbedOutput(tabs, "Damage received\t%s", commify(stat_damagereceived));

    C_TabbedOutput(tabs, "Deaths\t%s", commify(stat_deaths));

    C_TabbedOutput(tabs, "Cheated\t%s", commify(stat_cheated));

    C_TabbedOutput(tabs, "Shots fired\t%s", commify(stat_shotsfired));

    C_TabbedOutput(tabs, "Shots hit\t%s", commify(stat_shotshit));

    C_TabbedOutput(tabs, "Weapon accuracy\t%s%%",
        (stat_shotsfired ? striptrailingzero(stat_shotshit * 100.0f / stat_shotsfired, 1) : "0"));
}

//
// playerstats cmd
//
static void playerstats_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (gamestate == GS_LEVEL)
        C_PlayerStats_Game();
    else
        C_PlayerStats_NoGame();
}

//
// quit cmd
//
static void quit_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    I_Quit(true);
}

//
// respawnmonsters cmd
//
static dboolean respawnmonsters_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (skilllevel != sk_nightmare);
}

static void respawnmonsters_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, 1);

        if (value == 0)
            respawnparm = false;
        else if (value == 1)
            respawnparm = true;
    }
    else
        respawnparm = !respawnparm;

    HU_PlayerMessage((respawnparm ? s_STSTR_RMON : s_STSTR_RMOFF), false);
}

//
// resurrect cmd
//
static dboolean resurrect_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (gamestate == GS_LEVEL && players[0].playerstate == PST_DEAD);
}

static void resurrect_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    P_ResurrectPlayer(&players[0]);
}

//
// save cmd
//
static dboolean save_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (*parm1 && gamestate == GS_LEVEL && players[0].playerstate == PST_LIVE);
}

static void save_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    G_SaveGame(-1, "", M_StringJoin(savegamefolder, parm1, (M_StringEndsWith(parm1, ".save") ? "" :
        ".save"), NULL));
}

//
// spawn cmd
//
static int      spawncmdtype = NUMMOBJTYPES;

static dboolean spawn_cmd_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    char        *parm = M_StringJoin(parm1, parm2, parm3, NULL);

    if (!*parm)
        return true;

    if (gamestate == GS_LEVEL)
    {
        int i;

        for (i = 0; i < NUMMOBJTYPES; i++)
        {
            spawncmdtype = mobjinfo[i].doomednum;
            if (spawncmdtype >= 0 && (M_StringCompare(parm, removespaces(mobjinfo[i].name1))
                || (*mobjinfo[i].name2 && M_StringCompare(parm, removespaces(mobjinfo[i].name2)))
                || atoi(parm) == spawncmdtype))
            {
                dboolean        spawn = true;

                if (gamemode != commercial)
                {
                    switch (spawncmdtype)
                    {
                        case Arachnotron:
                        case ArchVile:
                        case BossBrain:
                        case HellKnight:
                        case Mancubus:
                        case PainElemental:
                        case HeavyWeaponDude:
                        case Revenant:
                        case WolfensteinSS:
                            spawn = false;
                            break;
                    }
                }
                else if (spawncmdtype == WolfensteinSS && bfgedition)
                    spawncmdtype = Zombieman;

                return spawn;
            }
        }
    }
    return false;
}

static void spawn_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    char        *parm = M_StringJoin(parm1, parm2, parm3, NULL);

    if (!*parm)
    {
        C_Output("%s %s", cmd, SPAWNCMDFORMAT);
        return;
    }
    else
    {
        mobj_t      *player = players[0].mo;
        fixed_t     x = player->x;
        fixed_t     y = player->y;
        angle_t     angle = player->angle >> ANGLETOFINESHIFT;
        mobj_t      *thing = P_SpawnMobj(x + 100 * finecosine[angle], y + 100 * finesine[angle],
            ONFLOORZ, P_FindDoomedNum(spawncmdtype));
        int         flags = thing->flags;

        thing->angle = R_PointToAngle2(thing->x, thing->y, x, y);

        if (flags & MF_COUNTKILL)
        {
            ++totalkills;
            ++monstercount[thing->type];
        }
        else if (flags & MF_COUNTITEM)
            ++totalitems;
    }
}

//
// thinglist cmd
//
static void thinglist_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    thinker_t   *th;
    int         count = 0;
    int         tabs[8] = { 45, 250, 0, 0, 0, 0, 0, 0 };

    for (th = thinkerclasscap[th_mobj].cnext; th != &thinkerclasscap[th_mobj]; th = th->cnext)
    {
        mobj_t      *mobj = (mobj_t *)th;

        C_TabbedOutput(tabs, "%i.\t%s\t(%i,%i,%i)", ++count, mobjinfo[mobj->type].name1,
            mobj->x >> FRACBITS, mobj->y >> FRACBITS, mobj->z >> FRACBITS);
    }
}

//
// unbind cmd
//
static void unbind_cmd_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    C_Bind(cmd, parm1, "none", "");
}

//
// dboolean cvars
//
static dboolean bool_cvars_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (!*parm1 || C_LookupValueFromAlias(parm1, BOOLALIAS) >= 0);
}

static void bool_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;

    while (*consolecmds[i].name)
    {
        if (M_StringCompare(cmd, consolecmds[i].name) && consolecmds[i].type == CT_CVAR
            && (consolecmds[i].flags & CF_BOOLEAN))
        {
            if (*parm1 && !(consolecmds[i].flags & CF_READONLY))
            {
                int     value = C_LookupValueFromAlias(parm1, BOOLALIAS);

                if (value == 0 || value == 1)
                {
                    *(dboolean *)consolecmds[i].variable = !!value;
                    M_SaveCVARs();
                }
            }
            else
                C_Output(C_LookupAliasFromValue(*(dboolean *)consolecmds[i].variable, BOOLALIAS));
        }
        ++i;
    }
}

//
// color cvars
//
static void color_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int_cvars_func2(cmd, parm1, parm2, parm3);
    if (*parm1)
        AM_setColors();
}

//
// float cvars
//
static dboolean float_cvars_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;

    if (!*parm1)
        return true;

    while (*consolecmds[i].name)
    {
        if (M_StringCompare(cmd, consolecmds[i].name) && consolecmds[i].type == CT_CVAR
            && (consolecmds[i].flags & CF_FLOAT))
        {
            float       value = -1.0f;

            sscanf(parm1, "%10f", &value);

            return (value >= 0.0f);
        }
        ++i;
    }
    return false;
}

static void float_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;

    while (*consolecmds[i].name)
    {
        if (M_StringCompare(cmd, consolecmds[i].name) && consolecmds[i].type == CT_CVAR
            && (consolecmds[i].flags & CF_FLOAT))
        {
            if (*parm1 && !(consolecmds[i].flags & CF_READONLY))
            {
                float     value = -1.0f;

                sscanf(parm1, "%10f", &value);

                if (value >= 0.0f)
                {
                    *(float *)consolecmds[i].variable = value;
                    M_SaveCVARs();
                }
            }
            else
                C_Output(striptrailingzero(*(float *)consolecmds[i].variable, 2));
        }
        ++i;
    }
}

//
// int cvars
//
static dboolean int_cvars_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;

    if (!*parm1)
        return true;

    while (*consolecmds[i].name)
    {
        if (M_StringCompare(cmd, consolecmds[i].name) && consolecmds[i].type == CT_CVAR
            && (consolecmds[i].flags & CF_INTEGER))
        {
            int value = -1;

            sscanf(parm1, "%10i", &value);

            return (value >= consolecmds[i].minimumvalue && value <= consolecmds[i].maximumvalue);
        }
        ++i;
    }
    return false;
}

static void int_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;

    while (*consolecmds[i].name)
    {
        if (M_StringCompare(cmd, consolecmds[i].name) && consolecmds[i].type == CT_CVAR
            && (consolecmds[i].flags & CF_INTEGER))
        {
            if (*parm1 && !(consolecmds[i].flags & CF_READONLY))
            {
                int     value = C_LookupValueFromAlias(parm1, consolecmds[i].aliases);

                if (value < 0)
                    sscanf(parm1, "%10i", &value);

                if (value >= 0)
                {
                    *(int *)consolecmds[i].variable = value;
                    M_SaveCVARs();
                }
            }
            else if (consolecmds[i].flags & CF_PERCENT)
                C_Output("%i%%", *(int *)consolecmds[i].variable);
            else
                C_Output(C_LookupAliasFromValue(*(int *)consolecmds[i].variable,
                    consolecmds[i].aliases));
        }
        ++i;
    }
}

//
// str cvars
//
static void str_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;

    while (*consolecmds[i].name)
    {
        if (M_StringCompare(cmd, consolecmds[i].name) && consolecmds[i].type == CT_CVAR
            && (consolecmds[i].flags & CF_STRING))
        {
            if (M_StringCompare(parm1, EMPTYVALUE))
            {
                *(char **)consolecmds[i].variable = "";
                M_SaveCVARs();
            }
            else if (*parm1)
            {
                *(char **)consolecmds[i].variable = strdup(parm1);
                M_SaveCVARs();
            }
            else
                C_Output("\"%s\"", *(char **)consolecmds[i].variable);
            break;
        }
        ++i;
    }
}

//
// time cvars
//
static void time_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int i = 0;

    while (*consolecmds[i].name)
    {
        if (M_StringCompare(cmd, consolecmds[i].name) && consolecmds[i].type == CT_CVAR
            && (consolecmds[i].flags & CF_TIME))
        {
            int tics = *(int *)consolecmds[i].variable / TICRATE;

            C_Output("%02i:%02i:%02i", tics / 3600, (tics % 3600) / 60, (tics % 3600) % 60);
        }
        ++i;
    }
}

//
// alwaysrun cvar
//
static void alwaysrun_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    bool_cvars_func2(cmd, parm1, "", "");
    I_InitKeyboard();
}

//
// am_external cvar
//
static void am_external_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    dboolean    am_external_old = am_external;

    bool_cvars_func2(cmd, parm1, "", "");
    if (am_external != am_external_old)
    {
        if (am_external)
        {
            I_CreateExternalAutoMap(false);
            if (gamestate == GS_LEVEL)
                AM_Start(false);
        }
        else
        {
            I_DestroyExternalAutoMap();
            if (gamestate == GS_LEVEL)
                AM_Stop();
        }
    }
}

//
// gp_deadzone_left and gp_deadzone_right cvars
//
static dboolean gp_deadzone_cvars_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    float value;

    if (!*parm1)
        return true;
    if (parm1[strlen(parm1) - 1] == '%')
        parm1[strlen(parm1) - 1] = 0;
    return sscanf(parm1, "%10f", &value);
}

static void gp_deadzone_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        float   value = 0;

        if (parm1[strlen(parm1) - 1] == '%')
            parm1[strlen(parm1) - 1] = 0;
        sscanf(parm1, "%10f", &value);

        if (M_StringCompare(cmd, stringize(gp_deadzone_left)))
        {
            gp_deadzone_left = BETWEENF(gp_deadzone_left_min, value, gp_deadzone_left_max);
            gamepadleftdeadzone = (short)(gp_deadzone_left * (float)SHRT_MAX / 100.0f);
        }
        else
        {
            gp_deadzone_right = BETWEENF(gp_deadzone_right_min, value, gp_deadzone_right_max);
            gamepadrightdeadzone = (short)(gp_deadzone_right * (float)SHRT_MAX / 100.0f);
        }
        M_SaveCVARs();
    }
    else
        C_Output("%s%%", striptrailingzero((M_StringCompare(cmd, stringize(gp_deadzone_left)) ?
            gp_deadzone_left : gp_deadzone_right), 1));
}

static void gp_sensitivity_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = -1;

        sscanf(parm1, "%10i", &value);

        if (value >= gp_sensitivity_min && value <= gp_sensitivity_max)
        {
            gp_sensitivity = value;
            M_SaveCVARs();
            I_SetGamepadSensitivity(gp_sensitivity);
        }
    }
    else
        C_Output("%i", gp_sensitivity);
}

//
// ammo, armor and health cvars
//
dboolean P_CheckAmmo(player_t *player);

static void player_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    player_t    *player = &players[0];
    int         value = -1;

    if (M_StringCompare(cmd, stringize(ammo)))
    {
        ammotype_t      ammotype = weaponinfo[player->readyweapon].ammo;

        if (*parm1)
        {
            sscanf(parm1, "%10i", &value);

            if (value >= 0 && player->playerstate == PST_LIVE && ammotype != am_noammo)
            {
                player->ammo[ammotype] = MIN(value, player->maxammo[ammotype]);
                P_CheckAmmo(player);
                if (player->pendingweapon != wp_nochange)
                    C_HideConsole();
            }
        }
        else
            C_Output("%i", player->ammo[ammotype]);
    }
    else if (M_StringCompare(cmd, stringize(armor)))
    {
        if (*parm1)
        {
            sscanf(parm1, "%10i", &value);

            if (value >= 0)
                player->armorpoints = MIN(value, max_armor);
        }
        else
            C_Output("%i%%", player->armorpoints);
    }
    else if (M_StringCompare(cmd, stringize(health)))
    {
        if (*parm1)
        {
            sscanf(parm1, "%10i", &value);

            if (value >= 0 && player->playerstate == PST_LIVE && !(player->cheats & CF_GODMODE)
                && !player->powers[pw_invulnerability])
            {
                player->health = player->mo->health = MIN(value, maxhealth);
                if (!value)
                {
                    P_KillMobj(player->mo, player->mo);
                    C_HideConsole();
                }
            }
        }
        else
            C_Output("%i%%", player->health);
    }
}

//
// playername cvar
//
static void playername_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        if (!M_StringCompare(parm1, EMPTYVALUE))
        {
            playername = strdup(parm1);
            M_SaveCVARs();
        }
    }
    else
        C_Output("\"%s\"", playername);
}

//
// r_blood cvar
//
static dboolean r_blood_cvar_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (!*parm1 || C_LookupValueFromAlias(parm1, BLOODALIAS) >= 0);
}

void (*P_BloodSplatSpawner)(fixed_t, fixed_t, int, int, mobj_t *);

static void r_blood_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, BLOODALIAS);

        if (value >= 0)
        {
            r_blood = value;
            P_BloodSplatSpawner = (r_blood == r_blood_none ? P_NullBloodSplatSpawner :
                P_SpawnBloodSplat);
            M_SaveCVARs();
        }
    }
    else
        C_Output(C_LookupAliasFromValue(r_blood, BLOODALIAS));
}

//
// r_detail cvar
//
static dboolean r_detail_cvar_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (!*parm1 || C_LookupValueFromAlias(parm1, DETAILALIAS) >= 0);
}

static void r_detail_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int value = C_LookupValueFromAlias(parm1, DETAILALIAS);

        if (value == 0 || value == 1)
        {
            r_detail = !!value;
            M_SaveCVARs();
        }
    }
    else
        C_Output(C_LookupAliasFromValue(r_detail, DETAILALIAS));
}

//
// r_gamma cvar
//
extern int      st_palette;

static dboolean r_gamma_cvar_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    float       value = -1.0f;

    if (!*parm1 || M_StringCompare(parm1, "off"))
        return true;

    sscanf(parm1, "%10f", &value);

    return (value >= 0.0f);
}

static void r_gamma_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        float   value = -1.0f;

        if (M_StringCompare(parm1, "off"))
            value = 1.0f;
        else
            sscanf(parm1, "%10f", &value);

        if (value >= 0.0f)
        {
            r_gamma = BETWEENF(r_gamma_min, value, r_gamma_max);
            I_SetGamma(r_gamma);
            I_SetPalette((byte *)W_CacheLumpName("PLAYPAL", PU_CACHE) + st_palette * 768);
            M_SaveCVARs();
        }
    }
    else
        C_Output(r_gamma == 1.0f ? "off" : striptrailingzero(r_gamma, 2));
}

//
// r_hud cvar
//
static void r_hud_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (vid_widescreen || r_screensize == r_screensize_max)
        bool_cvars_func2(cmd, parm1, "", "");
}

//
// r_lowpixelsiz cvar
//
static void r_lowpixelsize_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        r_lowpixelsize = strdup(parm1);

        GetPixelSize();

        if (!M_StringCompare(r_lowpixelsize, parm1))
            M_SaveCVARs();
    }
    else
        C_Output(formatsize(r_lowpixelsize));
}

//
// r_screensize cvar
//
static void r_screensize_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = -1;

        sscanf(parm1, "%10i", &value);

        if (value >= r_screensize_min && value <= r_screensize_max)
        {
            if (vid_widescreen || (returntowidescreen && gamestate != GS_LEVEL))
            {
                if (value == r_screensize_max)
                    --value;
                else if (value <= r_screensize_max - 1)
                {
                    r_hud = true;
                    I_ToggleWidescreen(false);
                }
            }
            else
            {
                if (value >= r_screensize_max - 1)
                {
                    if (gamestate != GS_LEVEL)
                    {
                        returntowidescreen = true;
                        r_hud = true;
                    }
                    else
                    {
                        I_ToggleWidescreen(true);
                        if (vid_widescreen)
                            value = r_screensize_max - 1;
                    }
                }
            }
            r_screensize = value;
            M_SaveCVARs();
            R_SetViewSize(r_screensize);
        }
    }
    else
        C_Output("%i", r_screensize);
}

//
// r_translucency cvar
//
static void r_translucency_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, BOOLALIAS);

        if ((value == 0 || value == 1) && value != r_translucency)
        {
            int i;

            r_translucency = !!value;
            M_SaveCVARs();
            R_InitColumnFunctions();

            for (i = 0; i < numsectors; ++i)
            {
                mobj_t   *mo = sectors[i].thinglist;

                while (mo)
                {
                    mobjtype_t  type = mo->type;

                    if (type == MT_BLOODSPLAT)
                        mo->colfunc = bloodsplatcolfunc;
                    else if (type == MT_SHADOW)
                        mo->colfunc = (mo->shadow->type == MT_SHADOWS ? R_DrawSpectreShadowColumn :
                            (r_translucency ? R_DrawShadowColumn : R_DrawSolidShadowColumn));
                    else
                        mo->colfunc = mo->info->colfunc;
                    mo = mo->snext;
                }
            }
        }
    }
    else
        C_Output(C_LookupAliasFromValue(r_translucency, BOOLALIAS));
}

//
// s_musicvolume and s_sfxvolume cvars
//
static dboolean s_volume_cvars_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int value = -1;

    if (!*parm1)
        return true;
    if (parm1[strlen(parm1) - 1] == '%')
        parm1[strlen(parm1) - 1] = 0;

    sscanf(parm1, "%10i", &value);

    return ((M_StringCompare(cmd, stringize(s_musicvolume)) && value >= s_musicvolume_min
        && value <= s_musicvolume_max) || (M_StringCompare(cmd, stringize(s_sfxvolume))
        && value >= s_sfxvolume_min && value <= s_sfxvolume_max));
}

static void s_volume_cvars_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int value = 0;

        if (parm1[strlen(parm1) - 1] == '%')
            parm1[strlen(parm1) - 1] = 0;
        sscanf(parm1, "%10i", &value);

        if (M_StringCompare(cmd, stringize(s_musicvolume)))
        {
            s_musicvolume = value;
            musicVolume = (BETWEEN(s_musicvolume_min, s_musicvolume,
                s_musicvolume_max) * 15 + 50) / 100;
            S_SetMusicVolume((int)(musicVolume * (127.0f / 15.0f)));
        }
        else
        {
            s_sfxvolume = value;
            sfxVolume = (BETWEEN(s_sfxvolume_min, s_sfxvolume, s_sfxvolume_max) * 15 + 50) / 100;
            S_SetSfxVolume((int)(sfxVolume * (127.0f / 15.0f)));
        }

        M_SaveCVARs();
    }
    else
        C_Output("%i%%", (M_StringCompare(cmd, stringize(s_musicvolume)) ? s_musicvolume :
            s_sfxvolume));
}

//
// turbo cvar
//
static dboolean turbo_cvar_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    int value = -1;

    if (!*parm1)
        return true;

    sscanf(parm1, "%10i", &value);
    return (value >= turbo_min && value <= turbo_max);
}

static void turbo_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = -1;

        sscanf(parm1, "%10i", &value);

        if (value >= turbo_min && value <= turbo_max && value != turbo)
        {
            turbo = value;
            M_SaveCVARs();
            G_SetMovementSpeed(turbo);
        }
    }
    else
        C_Output("%i%%", turbo);
}

//
// vid_display cvar
//
static void vid_display_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = -1;

        sscanf(parm1, "%10i", &value);

        if (value >= vid_display_min && value <= vid_display_max && value != vid_display)
        {
            vid_display = value;
            M_SaveCVARs();
            I_RestartGraphics();
        }
    }
    else
        C_Output("%i", vid_display);
}

//
// vid_fullscreen cvar
//
static void vid_fullscreen_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, BOOLALIAS);

        if ((value == 0 || value == 1) && value != vid_fullscreen)
            I_ToggleFullscreen();
    }
    else
        C_Output(C_LookupAliasFromValue(vid_fullscreen, BOOLALIAS));
}

//
// vid_scaledriver cvar
//
static dboolean vid_scaledriver_cvar_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (!*parm1 || M_StringCompare(parm1, EMPTYVALUE)
        || M_StringCompare(parm1, vid_scaledriver_direct3d)
        || M_StringCompare(parm1, vid_scaledriver_opengl)
        || M_StringCompare(parm1, vid_scaledriver_software));
}

static void vid_scaledriver_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        if (M_StringCompare(parm1, EMPTYVALUE))
        {
            vid_scaledriver = "";
            M_SaveCVARs();
            I_RestartGraphics();
        }
        else if (!M_StringCompare(parm1, vid_scaledriver))
        {
            vid_scaledriver = strdup(parm1);
            M_SaveCVARs();
            I_RestartGraphics();
        }
    }
    else
        C_Output("\"%s\"", vid_scaledriver);
}

//
// vid_scalefilter cvar
//
static dboolean vid_scalefilter_cvar_func1(char *cmd, char *parm1, char *parm2, char *parm3)
{
    return (!*parm1 || M_StringCompare(parm1, vid_scalefilter_nearest)
        || M_StringCompare(parm1, vid_scalefilter_linear)
        || M_StringCompare(parm1, vid_scalefilter_nearest_linear));
}

static void vid_scalefilter_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        if (!M_StringCompare(parm1, vid_scalefilter))
        {
            vid_scalefilter = strdup(parm1);
            M_SaveCVARs();
            I_RestartGraphics();
        }
    }
    else
        C_Output("\"%s\"", vid_scalefilter);
}

//
// vid_screenresolution cvar
//
static void vid_screenresolution_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        vid_screenresolution = strdup(parm1);

        GetScreenResolution();

        if (!M_StringCompare(vid_screenresolution, parm1))
        {
            M_SaveCVARs();

            if (vid_fullscreen)
                I_RestartGraphics();
        }
    }
    else
        C_Output(formatsize(vid_screenresolution));
}

//
// vid_showfps cvar
//
static void vid_showfps_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, BOOLALIAS);

        if ((value == 0 || value == 1) && value != vid_showfps)
        {
            vid_showfps = !!value;
            I_UpdateBlitFunc(players[0].damagecount);
        }
    }
    else
        C_Output(C_LookupAliasFromValue(vid_showfps, BOOLALIAS));
}

//
// vid_vsync cvar
//
static void vid_vsync_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, BOOLALIAS);

        if ((value == 0 || value == 1) && value != vid_vsync)
        {
            vid_vsync = !!value;
            M_SaveCVARs();
            I_RestartGraphics();
        }
    }
    else
        C_Output(C_LookupAliasFromValue(vid_vsync, BOOLALIAS));
}

//
// vid_widescreen cvar
//
static void vid_widescreen_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        int     value = C_LookupValueFromAlias(parm1, BOOLALIAS);

        if ((value == 0 || value == 1) && value != vid_widescreen)
        {
            vid_widescreen = !!value;
            if (vid_widescreen)
            {
                if (gamestate == GS_LEVEL)
                {
                    I_ToggleWidescreen(true);
                    if (vid_widescreen)
                        S_StartSound(NULL, sfx_stnmov);
                }
                else
                {
                    returntowidescreen = true;
                    r_hud = true;
                }
            }
            else
            {
                I_ToggleWidescreen(false);
                if (!vid_widescreen)
                    S_StartSound(NULL, sfx_stnmov);
            }
            M_SaveCVARs();
        }
    }
    else
        C_Output(C_LookupAliasFromValue(vid_widescreen, BOOLALIAS));
}

//
// vid_windowposition cvar
//
static void vid_windowposition_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        vid_windowposition = strdup(parm1);

        GetWindowPosition();

        M_SaveCVARs();

        if (!vid_fullscreen)
            SDL_SetWindowPosition(window, windowx, windowy);
    }
    else
        C_Output(vid_windowposition);
}

//
// vid_windowsize cvar
//
static void vid_windowsize_cvar_func2(char *cmd, char *parm1, char *parm2, char *parm3)
{
    if (*parm1)
    {
        vid_windowsize = strdup(parm1);

        GetWindowSize();

        M_SaveCVARs();

        if (!vid_fullscreen)
            SDL_SetWindowSize(window, windowwidth, windowheight);
    }
    else
        C_Output(formatsize(vid_windowsize));
}
