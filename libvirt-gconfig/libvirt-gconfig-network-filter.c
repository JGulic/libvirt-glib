/*
 * libvirt-gconfig-network-filter.c: libvirt network filter configuration
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

#include "libvirt-gconfig/libvirt-gconfig.h"

#define GVIR_CONFIG_NETWORK_FILTER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_NETWORK_FILTER, GVirConfigNetworkFilterPrivate))

struct _GVirConfigNetworkFilterPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigNetworkFilter, gvir_config_network_filter, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_network_filter_class_init(GVirConfigNetworkFilterClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigNetworkFilterPrivate));
}


static void gvir_config_network_filter_init(GVirConfigNetworkFilter *conn)
{
    g_debug("Init GVirConfigNetworkFilter=%p", conn);

    conn->priv = GVIR_CONFIG_NETWORK_FILTER_GET_PRIVATE(conn);
}


GVirConfigNetworkFilter *gvir_config_network_filter_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_NETWORK_FILTER,
                                    "filter",
                                    DATADIR "/libvirt/schemas/nwfilter.rng");
    return GVIR_CONFIG_NETWORK_FILTER(object);
}

GVirConfigNetworkFilter *gvir_config_network_filter_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_NETWORK_FILTER,
                                             "filter",
                                             DATADIR "/libvirt/schemas/nwfilter.rng",
                                             xml, error);
    return GVIR_CONFIG_NETWORK_FILTER(object);
}
