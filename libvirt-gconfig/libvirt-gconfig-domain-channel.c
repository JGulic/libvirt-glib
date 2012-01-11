/*
 * libvirt-gconfig-domain-channel.c: libvirt domain channel configuration
 *
 * Copyright (C) 2011-2012 Red Hat, Inc.
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

#define GVIR_CONFIG_DOMAIN_CHANNEL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHANNEL, GVirConfigDomainChannelPrivate))

struct _GVirConfigDomainChannelPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainChannel, gvir_config_domain_channel, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV);


static void gvir_config_domain_channel_class_init(GVirConfigDomainChannelClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainChannelPrivate));
}


static void gvir_config_domain_channel_init(GVirConfigDomainChannel *channel)
{
    g_debug("Init GVirConfigDomainChannel=%p", channel);

    channel->priv = GVIR_CONFIG_DOMAIN_CHANNEL_GET_PRIVATE(channel);
}

GVirConfigDomainChannel *gvir_config_domain_channel_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CHANNEL,
                                    "channel", NULL);
    return GVIR_CONFIG_DOMAIN_CHANNEL(object);
}

GVirConfigDomainChannel *gvir_config_domain_channel_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CHANNEL,
                                             "channel", NULL, xml, error);
    if (object == NULL)
        return NULL;
    return GVIR_CONFIG_DOMAIN_CHANNEL(object);
}
