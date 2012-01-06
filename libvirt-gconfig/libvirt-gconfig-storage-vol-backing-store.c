/*
 * libvirt-gconfig-storage-vol-backing-store.c: libvirt storage vol backing store configuration
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
#include "libvirt-gconfig/libvirt-gconfig-private.h"


#define GVIR_CONFIG_STORAGE_VOL_BACKING_STORE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_STORAGE_VOL_BACKING_STORE, GVirConfigStorageVolBackingStorePrivate))

struct _GVirConfigStorageVolBackingStorePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigStorageVolBackingStore, gvir_config_storage_vol_backing_store, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_storage_vol_backing_store_class_init(GVirConfigStorageVolBackingStoreClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigStorageVolBackingStorePrivate));
}


static void gvir_config_storage_vol_backing_store_init(GVirConfigStorageVolBackingStore *backing_store)
{
    g_debug("Init GVirConfigStorageVolBackingStore=%p", backing_store);

    backing_store->priv = GVIR_CONFIG_STORAGE_VOL_BACKING_STORE_GET_PRIVATE(backing_store);
}


GVirConfigStorageVolBackingStore *gvir_config_storage_vol_backing_store_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_STORAGE_VOL_BACKING_STORE,
                                    "backingStore", NULL);
    return GVIR_CONFIG_STORAGE_VOL_BACKING_STORE(object);
}

GVirConfigStorageVolBackingStore *gvir_config_storage_vol_backing_store_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_STORAGE_VOL_BACKING_STORE,
                                             "backingStore", NULL,
                                             xml, error);
    return GVIR_CONFIG_STORAGE_VOL_BACKING_STORE(object);
}

void gvir_config_storage_vol_backing_store_set_format(GVirConfigStorageVolBackingStore *backing_store,
                                                      const char *format)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_STORAGE_VOL_BACKING_STORE(backing_store));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(backing_store), "format");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute(node, "type", format, NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_storage_vol_backing_store_set_path(GVirConfigStorageVolBackingStore *backing_store,
                                                    const char *path)
{
    g_return_if_fail(GVIR_IS_CONFIG_STORAGE_VOL_BACKING_STORE(backing_store));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(backing_store),
                                        "path", path);
}
