/*
 * libvirt-gconfig-main.c: libvirt gconfig integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gconfig/libvirt-gconfig.h"

/**
 * gvir_init_config:
 * @argc: (inout): pointer to application's argc
 * @argv: (inout) (array length=argc) (allow-none): pointer to application's argv
 */
void gvir_init_config(int *argc,
                      char ***argv)
{
    GError *err = NULL;
    if (!gvir_init_config_check(argc, argv, &err)) {
        g_error("Could not initialize libvirt-gconfig: %s\n",
                err->message);
    }
}

static void gvir_log_handler(const gchar *log_domain G_GNUC_UNUSED,
                             GLogLevelFlags log_level G_GNUC_UNUSED,
                             const gchar *message,
                             gpointer user_data)
{
    if (user_data)
        fprintf(stderr, "%s\n", message);
}


/**
 * gvir_init_config_check:
 * @argc: (inout): pointer to application's argc
 * @argv: (inout) (array length=argc) (allow-none): pointer to application's argv
 * @err: pointer to a #GError to which a message will be posted on error
 */
gboolean gvir_init_config_check(int *argc G_GNUC_UNUSED,
                                char ***argv G_GNUC_UNUSED,
                                GError **err G_GNUC_UNUSED)
{
    g_type_init();

    /* GLib >= 2.31.0 debug is off by default, so we need to
     * enable it. Older versions are on by default, so we need
     * to disable it.
     */
#if GLIB_CHECK_VERSION(2, 31, 0)
    if (getenv("LIBVIRT_GCONFIG_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          gvir_log_handler, (void*)0x1);
#else
    if (!getenv("LIBVIRT_GCONFIG_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          gvir_log_handler, NULL);
#endif

    return TRUE;
}

