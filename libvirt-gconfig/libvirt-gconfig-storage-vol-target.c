/*
 * libvirt-gconfig-storage-vol-target.c: libvirt storage vol target configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"


#define GVIR_CONFIG_STORAGE_VOL_TARGET_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_STORAGE_VOL_TARGET, GVirConfigStorageVolTargetPrivate))

struct _GVirConfigStorageVolTargetPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigStorageVolTarget, gvir_config_storage_vol_target, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_storage_vol_target_class_init(GVirConfigStorageVolTargetClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigStorageVolTargetPrivate));
}


static void gvir_config_storage_vol_target_init(GVirConfigStorageVolTarget *target)
{
    g_debug("Init GVirConfigStorageVolTarget=%p", target);

    target->priv = GVIR_CONFIG_STORAGE_VOL_TARGET_GET_PRIVATE(target);
}


GVirConfigStorageVolTarget *gvir_config_storage_vol_target_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_STORAGE_VOL_TARGET,
                                    "target", NULL);
    return GVIR_CONFIG_STORAGE_VOL_TARGET(object);
}

GVirConfigStorageVolTarget *gvir_config_storage_vol_target_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_STORAGE_VOL_TARGET,
                                             "target", NULL,
                                             xml, error);
    return GVIR_CONFIG_STORAGE_VOL_TARGET(object);
}

void gvir_config_storage_vol_target_set_format(GVirConfigStorageVolTarget *target,
                                               const char *format)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_STORAGE_VOL_TARGET(target));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(target), "format");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute(node, "type", format, NULL);
    g_object_unref(G_OBJECT(node));
}
