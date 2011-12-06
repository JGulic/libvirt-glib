/*
 * libvirt-gconfig-domain-console.c: libvirt domain console configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
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

#include "libvirt-gconfig/libvirt-gconfig.h"

#define GVIR_CONFIG_DOMAIN_CONSOLE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_CONSOLE, GVirConfigDomainConsolePrivate))

struct _GVirConfigDomainConsolePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainConsole, gvir_config_domain_console, GVIR_TYPE_CONFIG_DOMAIN_CHARDEV);


static void gvir_config_domain_console_class_init(GVirConfigDomainConsoleClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainConsolePrivate));
}


static void gvir_config_domain_console_init(GVirConfigDomainConsole *console)
{
    g_debug("Init GVirConfigDomainConsole=%p", console);

    console->priv = GVIR_CONFIG_DOMAIN_CONSOLE_GET_PRIVATE(console);
}

GVirConfigDomainConsole *gvir_config_domain_console_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_CONSOLE,
                                    "console", NULL);
    return GVIR_CONFIG_DOMAIN_CONSOLE(object);
}

GVirConfigDomainConsole *gvir_config_domain_console_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_CONSOLE,
                                             "console", NULL, xml, error);
    if (object == NULL)
        return NULL;
    return GVIR_CONFIG_DOMAIN_CONSOLE(object);
}
