/*
 * Scoring Plugin for Audacious by viking
 *
 * Copyright 2013 Jeremy Stephens <viking@pillageandplunder.net>
 *
 * This plugin is part of the Audacious Media Player.
 * It is licensed under the GNU General Public License, version 3.
 */

#ifndef SCORING_H_
#define SCORING_H_

//external includes
#include <string.h>
#include <time.h>
#include <glib.h>

//audacious includes
#include <audacious/plugin.h>
#include <audacious/playlist.h>
#include <audacious/drct.h>
#include <audacious/i18n.h>
#include <audacious/misc.h>
#include <audacious/debug.h>
#include <audacious/preferences.h>
#include <libaudcore/hook.h>
#include <libaudcore/audstrings.h>

enum scoring_event_type {
  SCORING_EVENT_NONE = 0,
  SCORING_EVENT_STOP,
  SCORING_EVENT_END
};

#endif /* SCROBBLER_H_ */
