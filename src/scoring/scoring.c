/*
 * Scoring Plugin for Audacious by viking
 *
 * Copyright 2013 Jeremy Stephens <viking@pillageandplunder.net>
 *
 * This plugin is part of the Audacious Media Player.
 * It is licensed under the GNU General Public License, version 3.
 */

#include "scoring.h"

static Tuple *current_track = NULL;
static gint current_time, current_length;
static guint scoring_time_counter_source = 0;

static void scoring_cleanup_track() {
  if (current_track != NULL) {
    tuple_unref(current_track);
    current_track = NULL;
  }
  if (scoring_time_counter_source) {
    g_source_remove(scoring_time_counter_source);
    scoring_time_counter_source = 0;
  }
}

static gboolean scoring_time_counter(void)
{
  current_time = aud_drct_get_time();
  current_length = aud_drct_get_length();

  return TRUE;
}

static void scoring_log(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *logpath = g_strconcat(aud_get_path(AUD_PATH_USER_DIR), "/scoring.log", NULL);
  FILE *f = fopen(logpath, "a");
  if (f == NULL) {
    perror("fopen");
  }
  else {
    if (vfprintf(f, format, args) < 0) {
      perror("vfprintf");
    }
  }
  fclose(f);
  g_free(logpath);

  va_end(args);
}

static void scoring_event(const char *which, enum scoring_event_type type) {
  if (current_track == NULL) {
    AUDDBG("%s: current track was NULL\n", which);
    return;
  }

  char *title = tuple_get_str(current_track, FIELD_TITLE, NULL);
  char *album = tuple_get_str(current_track, FIELD_ALBUM, NULL);
  char *artist = tuple_get_str(current_track, FIELD_ARTIST, NULL);
  AUDDBG("%s: %s - %s - %s, time: %d, length: %d\n", which,
      artist ? artist : "Unknown",
      album ? album : "Unknown",
      title ? title : "Unknown",
      current_time, current_length);

  if (type != SCORING_EVENT_NONE && title && artist) {
    time_t now = time(NULL);
    int score = (type == SCORING_EVENT_END) ? 100 :
      current_time * 100 / current_length;

    char *title2 = g_strdup(title);
    string_replace_char(title2, '\t', ' ');

    char *artist2 = g_strdup(artist);
    string_replace_char(artist2, '\t', ' ');

    char *album2 = NULL;
    if (album) {
      album2 = g_strdup(album);
      string_replace_char(album2, '\t', ' ');
    }

    scoring_log("%d\t%s\t%s\t%s\t%d\n", now, artist2, album2, title2, score);

    g_free(title2);
    g_free(artist2);
    if (album2) {
      g_free(album2);
    }
  }

  if (title) {
    str_unref(title);
  }
  if (album) {
    str_unref(album);
  }
  if (artist) {
    str_unref(artist);
  }
}

static void scoring_playback_begin(void *hook_data, void *user_data) {
  if (current_track != NULL) {
    scoring_event("advance", SCORING_EVENT_STOP);
    scoring_cleanup_track();
  }
  current_track = aud_playlist_entry_get_tuple(aud_playlist_get_playing(), aud_playlist_get_position(aud_playlist_get_playing()), FALSE);

  scoring_time_counter();
  if (!scoring_time_counter_source) {
    scoring_time_counter_source = g_timeout_add(250, (GSourceFunc)
        scoring_time_counter, NULL);
  }

  scoring_event("begin", SCORING_EVENT_NONE);
}

static void scoring_playback_ended(void *hook_data, void *user_data) {
  scoring_event("ended", SCORING_EVENT_END);
  scoring_cleanup_track();
}

static void scoring_playback_stopped(void *hook_data, void *user_data) {
  scoring_event("stopped", SCORING_EVENT_STOP);
  scoring_cleanup_track();
}

static bool_t scoring_init (void) {
  hook_associate("playback begin", (HookFunction) scoring_playback_begin, NULL);
  hook_associate("playback end", (HookFunction) scoring_playback_ended, NULL);
  hook_associate("playback stop", (HookFunction) scoring_playback_stopped, NULL);
  return TRUE;
}

static void scoring_cleanup (void) {
  hook_dissociate("playback begin", (HookFunction) scoring_playback_begin);
  hook_dissociate("playback end", (HookFunction) scoring_playback_ended);
  hook_dissociate("playback stop", (HookFunction) scoring_playback_stopped);
}

static const char scoring_about[] =
N_("Audacious Scoring Plugin by viking,\n\n"
    "Copyright © 2013 Jeremy Stephens <viking@pillageandplunder.net>\n\n");

AUD_GENERAL_PLUGIN(
    .name = N_("Scoring"),
    .domain = PACKAGE,
    .about_text = scoring_about,
    .init = scoring_init,
    .cleanup = scoring_cleanup
    )
