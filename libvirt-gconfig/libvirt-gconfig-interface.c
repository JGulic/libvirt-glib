/*
 * libvirt-gobject-config_interface.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010 Red Hat
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

#define GVIR_CONFIG_INTERFACE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_INTERFACE, GVirConfigInterfacePrivate))

struct _GVirConfigInterfacePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigInterface, gvir_config_interface, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_interface_class_init(GVirConfigInterfaceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigInterfacePrivate));
}


static void gvir_config_interface_init(GVirConfigInterface *conn)
{
    g_debug("Init GVirConfigInterface=%p", conn);

    conn->priv = GVIR_CONFIG_INTERFACE_GET_PRIVATE(conn);
}


GVirConfigInterface *gvir_config_interface_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_INTERFACE,
                                    "interface",
                                    DATADIR "/libvirt/schemas/interface.rng");
    return GVIR_CONFIG_INTERFACE(object);
}

GVirConfigInterface *gvir_config_interface_new_from_xml(const gchar *xml,
                                                        GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_INTERFACE,
                                             "interface",
                                             DATADIR "/libvirt/schemas/interface.rng",
                                             xml, error);
    return GVIR_CONFIG_INTERFACE(object);
}

