/*
 * libvirt-gconfig-domain-disk.h: libvirt domain disk configuration
 *
 * Copyright (C) 2011 Red Hat
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_DISK_H__
#define __LIBVIRT_GCONFIG_DOMAIN_DISK_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_DOMAIN_DISK            (gvir_config_domain_disk_get_type ())
#define GVIR_CONFIG_DOMAIN_DISK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_DOMAIN_DISK, GVirConfigDomainDisk))
#define GVIR_CONFIG_DOMAIN_DISK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_DOMAIN_DISK, GVirConfigDomainDiskClass))
#define GVIR_IS_CONFIG_DOMAIN_DISK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_DOMAIN_DISK))
#define GVIR_IS_CONFIG_DOMAIN_DISK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_DOMAIN_DISK))
#define GVIR_CONFIG_DOMAIN_DISK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_DOMAIN_DISK, GVirConfigDomainDiskClass))

typedef struct _GVirConfigDomainDisk GVirConfigDomainDisk;
typedef struct _GVirConfigDomainDiskPrivate GVirConfigDomainDiskPrivate;
typedef struct _GVirConfigDomainDiskClass GVirConfigDomainDiskClass;

struct _GVirConfigDomainDisk
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainDiskPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainDiskClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_DISK_FILE,
    GVIR_CONFIG_DOMAIN_DISK_BLOCK,
    GVIR_CONFIG_DOMAIN_DISK_DIR,
    GVIR_CONFIG_DOMAIN_DISK_NETWORK
} GVirConfigDomainDiskType;

typedef enum {
    GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK,
    GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_FLOPPY,
    GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_CDROM
} GVirConfigDomainDiskGuestDeviceType;

typedef enum {
    GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_NO,
    GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_INTERNAL,
    GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_EXTERNAL
} GVirConfigDomainDiskSnapshotType;

GType gvir_config_domain_disk_get_type(void);

GVirConfigDomainDisk *gvir_config_domain_disk_new(void);
GVirConfigDomainDisk *gvir_config_domain_disk_new_from_xml(const gchar *xml,
                                                           GError **error);

void gvir_config_domain_disk_set_type(GVirConfigDomainDisk *disk,
                                      GVirConfigDomainDiskType type);
void gvir_config_domain_disk_set_guest_device_type(GVirConfigDomainDisk *disk,
                                                   GVirConfigDomainDiskGuestDeviceType type);
void gvir_config_domain_disk_set_snapshot_type(GVirConfigDomainDisk *disk,
                                               GVirConfigDomainDiskSnapshotType type);
void gvir_config_domain_disk_set_source(GVirConfigDomainDisk *disk,
                                        const char *source);
void gvir_config_domain_disk_set_driver_name(GVirConfigDomainDisk *disk,
                                             const char *driver_name);
void gvir_config_domain_disk_set_driver_type(GVirConfigDomainDisk *disk,
                                             const char *driver_type);
void gvir_config_domain_disk_set_target_bus(GVirConfigDomainDisk *disk,
                                            const char *bus);
void gvir_config_domain_disk_set_target_dev(GVirConfigDomainDisk *disk,
                                            const char *dev);
G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_DISK_H__ */
