/*
 * Audacious - a cross-platform multimedia player
 * Copyright (c) 2008 Tomasz Moń
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; under version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses>.
 *
 * The Audacious team does not consider modular code linking to
 * Audacious or using our public API to be a derived work.
 */


#include "plugin.h"
#include "skins_cfg.h"
#include "ui_skin.h"
#include "ui_skinned_window.h"
#include "ui_manager.h"
#include "icons-stock.h"
#include "ui_main_evlisteners.h"
#include "ui_playlist_evlisteners.h"
#include <audacious/i18n.h>
#include <libintl.h>

#define PACKAGE "audacious-plugins"

GeneralPlugin skins_gp =
{
    .description= "Audacious Skinned GUI",
    .init = skins_init,
    .about = skins_about,
    .configure = skins_configure,
    .cleanup = skins_cleanup
};

GeneralPlugin *skins_gplist[] = { &skins_gp, NULL };
SIMPLE_GENERAL_PLUGIN(skins, skins_gplist);
GtkWidget *mainwin;
gboolean plugin_is_active = FALSE;

void skins_init(void) {
    plugin_is_active = TRUE;
    g_log_set_handler(NULL, G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    skins_cfg_load();

    register_aud_stock_icons();
    ui_manager_init();
    ui_manager_create_menus();
    mainwin_setup_menus();

    init_skins(config.skin);

    if (config.player_visible) mainwin_real_show();
    if (config.equalizer_visible) equalizerwin_show(TRUE);
    if (config.playlist_visible) playlistwin_show();

    return;
}

void skins_cleanup(void) {
    if (plugin_is_active == TRUE) {
        skins_cfg_save();
        ui_main_evlistener_dissociate();
        ui_playlist_evlistener_dissociate();
        skins_cfg_free();
        gtk_widget_destroy(mainwin);
        gtk_widget_destroy(equalizerwin);
        skin_destroy(aud_active_skin);
        aud_active_skin = NULL;
        mainwin = NULL;
        equalizerwin = NULL;
        playlistwin = NULL;
        mainwin_info = NULL;
        plugin_is_active = FALSE;
    }

    return;
}


void skins_configure(void) {
    return;
}

void skins_about(void) {
    static GtkWidget* about_window = NULL;

    if (about_window) {
        gtk_window_present(GTK_WINDOW(about_window));
        return;
    }

    about_window = audacious_info_dialog(_("About Skinned GUI"),
                   _("Copyright (c) 2008, by Tomasz Moń <desowin@gmail.com>\n\n"),
                   _("OK"), FALSE, NULL, NULL);

    g_signal_connect(G_OBJECT(about_window), "destroy",	G_CALLBACK(gtk_widget_destroyed), &about_window);
}