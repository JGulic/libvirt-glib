/*
 * libvirt-gconfig-storage-pool-target.c: libvirt storage pool target configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_STORAGE_POOL_SOURCE_H__
#define __LIBVIRT_GCONFIG_STORAGE_POOL_SOURCE_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE            (gvir_config_storage_pool_source_get_type ())
#define GVIR_CONFIG_STORAGE_POOL_SOURCE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSource))
#define GVIR_CONFIG_STORAGE_POOL_SOURCE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSourceClass))
#define GVIR_IS_CONFIG_STORAGE_POOL_SOURCE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE))
#define GVIR_IS_CONFIG_STORAGE_POOL_SOURCE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE))
#define GVIR_CONFIG_STORAGE_POOL_SOURCE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSourceClass))

typedef struct _GVirConfigStoragePoolSource GVirConfigStoragePoolSource;
typedef struct _GVirConfigStoragePoolSourcePrivate GVirConfigStoragePoolSourcePrivate;
typedef struct _GVirConfigStoragePoolSourceClass GVirConfigStoragePoolSourceClass;

struct _GVirConfigStoragePoolSource
{
    GVirConfigObject parent;

    GVirConfigStoragePoolSourcePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStoragePoolSourceClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_pool_source_get_type(void);

GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new(void);
GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new_from_xml(const gchar *xml,
                                                                          GError **error);

void gvir_config_storage_pool_source_set_adapter(GVirConfigStoragePoolSource *source,
                                                 const char *adapter);
void gvir_config_storage_pool_source_set_device_path(GVirConfigStoragePoolSource *source,
                                                     const char *device_path);
void gvir_config_storage_pool_source_set_directory(GVirConfigStoragePoolSource *source,
                                                   const char *directory);
void gvir_config_storage_pool_source_set_format(GVirConfigStoragePoolSource *source,
                                                const char *format);
void gvir_config_storage_pool_source_set_host(GVirConfigStoragePoolSource *source,
                                              const char *host);
void gvir_config_storage_pool_source_set_name(GVirConfigStoragePoolSource *source,
                                              const char *name);
void gvir_config_storage_pool_source_set_product(GVirConfigStoragePoolSource *source,
                                                 const char *product);
void gvir_config_storage_pool_source_set_vendor(GVirConfigStoragePoolSource *source,
                                                const char *vendor);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_STORAGE_POOL_SOURCE_H__ */
