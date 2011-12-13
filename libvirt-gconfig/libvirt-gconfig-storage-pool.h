/*
 * libvirt-gconfig-storage-pool.h: libvirt storage pool configuration
 *
 * Copyright (C) 2010-2011 Red Hat, Inc.
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_STORAGE_POOL_H__
#define __LIBVIRT_GCONFIG_STORAGE_POOL_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_STORAGE_POOL            (gvir_config_storage_pool_get_type ())
#define GVIR_CONFIG_STORAGE_POOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_STORAGE_POOL, GVirConfigStoragePool))
#define GVIR_CONFIG_STORAGE_POOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_STORAGE_POOL, GVirConfigStoragePoolClass))
#define GVIR_IS_CONFIG_STORAGE_POOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_STORAGE_POOL))
#define GVIR_IS_CONFIG_STORAGE_POOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_STORAGE_POOL))
#define GVIR_CONFIG_STORAGE_POOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_STORAGE_POOL, GVirConfigStoragePoolClass))

typedef struct _GVirConfigStoragePool GVirConfigStoragePool;
typedef struct _GVirConfigStoragePoolPrivate GVirConfigStoragePoolPrivate;
typedef struct _GVirConfigStoragePoolClass GVirConfigStoragePoolClass;

struct _GVirConfigStoragePool
{
    GVirConfigObject parent;

    GVirConfigStoragePoolPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStoragePoolClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_pool_get_type(void);

GVirConfigStoragePool *gvir_config_storage_pool_new(void);
GVirConfigStoragePool *gvir_config_storage_pool_new_from_xml(const gchar *xml,
                                                             GError **error);

void gvir_config_storage_pool_set_allocation(GVirConfigStoragePool *pool,
                                             guint64 allocation);
void gvir_config_storage_pool_set_available(GVirConfigStoragePool *pool,
                                            guint64 available);
void gvir_config_storage_pool_set_capacity(GVirConfigStoragePool *pool,
                                           guint64 capacity);
void gvir_config_storage_pool_set_name(GVirConfigStoragePool *pool,
                                       const char *name);
void gvir_config_storage_pool_set_uuid(GVirConfigStoragePool *pool,
                                       const char *uuid);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_STORAGE_POOL_H__ */
