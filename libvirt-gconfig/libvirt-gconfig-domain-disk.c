/*
 * libvirt-gconfig-domain-disk.c: libvirt domain disk configuration
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_DISK, GVirConfigDomainDiskPrivate))

struct _GVirConfigDomainDiskPrivate
{
    GVirConfigDomainDiskType type;
};

G_DEFINE_TYPE(GVirConfigDomainDisk, gvir_config_domain_disk, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_disk_class_init(GVirConfigDomainDiskClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainDiskPrivate));
}


static void gvir_config_domain_disk_init(GVirConfigDomainDisk *disk)
{
    g_debug("Init GVirConfigDomainDisk=%p", disk);

    disk->priv = GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(disk);
}


GVirConfigDomainDisk *gvir_config_domain_disk_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_DISK,
                                    "disk", NULL);
    return GVIR_CONFIG_DOMAIN_DISK(object);
}

GVirConfigDomainDisk *gvir_config_domain_disk_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;
    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_DISK,
                                             "disk", NULL, xml, error);

    return GVIR_CONFIG_DOMAIN_DISK(object);
}

GVirConfigDomainDevice *
gvir_config_domain_disk_new_from_tree(GVirConfigXmlDoc *doc,
                                      xmlNodePtr tree)
{
    GVirConfigObject *object;
    GVirConfigDomainDisk *disk;
    GVirConfigDomainDiskType type;
    xmlChar *type_str;

    type_str = gvir_config_xml_get_attribute_content(tree, "type");
    if (type_str == NULL)
        return NULL;

    type = gvir_config_genum_get_value(GVIR_CONFIG_TYPE_DOMAIN_DISK_TYPE,
                                       (char *)type_str,
                                       GVIR_CONFIG_DOMAIN_DISK_FILE);
    xmlFree(type_str);
    if (type == -1)
        return NULL;

    object = gvir_config_object_new_from_tree(GVIR_CONFIG_TYPE_DOMAIN_DISK,
                                              doc, NULL, tree);
    disk = GVIR_CONFIG_DOMAIN_DISK(object);
    disk->priv->type = type;

    return GVIR_CONFIG_DOMAIN_DEVICE(object);
}

void gvir_config_domain_disk_set_type(GVirConfigDomainDisk *disk,
                                      GVirConfigDomainDiskType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_TYPE,
                                               type, NULL);
    disk->priv->type = type;
}

void gvir_config_domain_disk_set_guest_device_type(GVirConfigDomainDisk *disk,
                                                   GVirConfigDomainDiskGuestDeviceType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "device",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_GUEST_DEVICE_TYPE,
                                               type, NULL);
}

void gvir_config_domain_disk_set_snapshot_type(GVirConfigDomainDisk *disk,
                                               GVirConfigDomainDiskSnapshotType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "snapshot",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_SNAPSHOT_TYPE,
                                               type, NULL);
}

void gvir_config_domain_disk_set_source(GVirConfigDomainDisk *disk,
                                        const char *source)
{
    const char *attribute_name;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

    switch (disk->priv->type) {
        case GVIR_CONFIG_DOMAIN_DISK_FILE:
            attribute_name = "file";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_BLOCK:
            attribute_name = "dev";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_DIR:
            attribute_name = "dir";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_NETWORK:
            attribute_name = "protocol";
            break;
        default:
            g_return_if_reached();
    }
    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                   "source",
                                                   attribute_name, source);
}

void gvir_config_domain_disk_set_driver_name(GVirConfigDomainDisk *disk,
                                             const char *driver_name)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "driver", "name", driver_name);
}

void gvir_config_domain_disk_set_driver_type(GVirConfigDomainDisk *disk,
                                             const char *driver_type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "driver", "type", driver_type);
}

void gvir_config_domain_disk_set_target_bus(GVirConfigDomainDisk *disk,
                                            GVirConfigDomainDiskBus bus)
{
    const char *bus_str;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    bus_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_DISK_BUS, bus);
    g_return_if_fail(bus_str != NULL);
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "target", "bus", bus_str);
}

void gvir_config_domain_disk_set_target_dev(GVirConfigDomainDisk *disk,
                                            const char *dev)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "target", "dev", dev);
}

GVirConfigDomainDiskType
gvir_config_domain_disk_get_disk_type(GVirConfigDomainDisk *disk)
{
    return disk->priv->type;
}

GVirConfigDomainDiskGuestDeviceType
gvir_config_domain_disk_get_guest_device_type(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  NULL,
                                                  "device",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_GUEST_DEVICE_TYPE,
                                                  GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);
}

GVirConfigDomainDiskSnapshotType
gvir_config_domain_disk_get_snapshot_type(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_NO);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  NULL,
                                                  "snapshot",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_SNAPSHOT_TYPE,
                                                  GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_NO);
}

char *
gvir_config_domain_disk_get_source(GVirConfigDomainDisk *disk)
{
    const char *attribute_name;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    switch (disk->priv->type) {
        case GVIR_CONFIG_DOMAIN_DISK_FILE:
            attribute_name = "file";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_BLOCK:
            attribute_name = "dev";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_DIR:
            attribute_name = "dir";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_NETWORK:
            attribute_name = "protocol";
            break;
        default:
            g_return_val_if_reached(NULL);
    }
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "source", attribute_name);
}

char *
gvir_config_domain_disk_get_driver_name(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "driver", "name");
}

char *
gvir_config_domain_disk_get_driver_type(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "driver", "type");
}

GVirConfigDomainDiskBus
gvir_config_domain_disk_get_target_bus(GVirConfigDomainDisk *disk)
{
    /* FIXME: the default value depends on the "name" attribute, should we
     * copy what libvirt is doing here?
     */
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  "target", "snapshot",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_BUS,
                                                  GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);
}

char *
gvir_config_domain_disk_get_target_dev(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "target", "dev");
}

void
gvir_config_domain_disk_set_readonly(GVirConfigDomainDisk *disk,
                                     gboolean readonly)
{
    if (readonly) {
        GVirConfigObject *node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(disk), "readonly");
        g_object_unref(node);
    } else
        gvir_config_object_delete_child(GVIR_CONFIG_OBJECT(disk), "readonly", NULL);
}
