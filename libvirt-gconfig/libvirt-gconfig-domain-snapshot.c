/*
 * libvirt-gobject-config_domain_snapshot.c: libvirt glib integration
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

#define GVIR_CONFIG_DOMAIN_SNAPSHOT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_SNAPSHOT, GVirConfigDomainSnapshotPrivate))

struct _GVirConfigDomainSnapshotPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainSnapshot, gvir_config_domain_snapshot, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_domain_snapshot_class_init(GVirConfigDomainSnapshotClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainSnapshotPrivate));
}


static void gvir_config_domain_snapshot_init(GVirConfigDomainSnapshot *conn)
{
    g_debug("Init GVirConfigDomainSnapshot=%p", conn);

    conn->priv = GVIR_CONFIG_DOMAIN_SNAPSHOT_GET_PRIVATE(conn);
}


GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_SNAPSHOT,
                                    "domainsnapshot",
                                    DATADIR "/libvirt/schemas/domainsnapshot.rng");
    return GVIR_CONFIG_DOMAIN_SNAPSHOT(object);
}

GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new_from_xml(const gchar *xml,
                                                                   GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_SNAPSHOT,
                                    "domainsnapshot",
                                    DATADIR "/libvirt/schemas/domainsnapshot.rng",
                                    xml, error);
    return GVIR_CONFIG_DOMAIN_SNAPSHOT(object);
}
