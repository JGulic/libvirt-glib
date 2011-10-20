/*
 * libvirt-gobject-domain-os.c: libvirt gobject integration
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_OS_H__
#define __LIBVIRT_GCONFIG_DOMAIN_OS_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_DOMAIN_OS            (gvir_config_domain_os_get_type ())
#define GVIR_CONFIG_DOMAIN_OS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_DOMAIN_OS, GVirConfigDomainOs))
#define GVIR_CONFIG_DOMAIN_OS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_DOMAIN_OS, GVirConfigDomainOsClass))
#define GVIR_IS_CONFIG_DOMAIN_OS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_DOMAIN_OS))
#define GVIR_IS_CONFIG_DOMAIN_OS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_DOMAIN_OS))
#define GVIR_CONFIG_DOMAIN_OS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_DOMAIN_OS, GVirConfigDomainOsClass))

typedef struct _GVirConfigDomainOs GVirConfigDomainOs;
typedef struct _GVirConfigDomainOsPrivate GVirConfigDomainOsPrivate;
typedef struct _GVirConfigDomainOsClass GVirConfigDomainOsClass;

struct _GVirConfigDomainOs
{
    GVirConfigObject parent;

    GVirConfigDomainOsPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainOsClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_OS_TYPE_HVM,
    GVIR_CONFIG_DOMAIN_OS_TYPE_LINUX
} GVirConfigDomainOsType;

typedef enum {
    GVIR_CONFIG_DOMAIN_OS_SMBIOS_MODE_EMULATE,
    GVIR_CONFIG_DOMAIN_OS_SMBIOS_MODE_HOST,
    GVIR_CONFIG_DOMAIN_OS_SMBIOS_MODE_SYSINFO
} GVirConfigDomainOsSmBiosMode;

typedef enum {
    GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_FD,
    GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_HD,
    GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_CDROM,
    GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_NETWORK
} GVirConfigDomainOsBootDevice;

GType gvir_config_domain_os_get_type(void);

GVirConfigDomainOs *gvir_config_domain_os_new(void);
GVirConfigDomainOs *gvir_config_domain_os_new_from_xml(const gchar *xml, GError **error);

void gvir_config_domain_os_set_os_type(GVirConfigDomainOs *os, GVirConfigDomainOsType type);
void gvir_config_domain_os_set_arch(GVirConfigDomainOs *os, const char *arch);
void gvir_config_domain_os_set_boot_devices(GVirConfigDomainOs *os, GList *boot_devices);
void gvir_config_domain_os_set_loader(GVirConfigDomainOs *os, const char * loader);
void gvir_config_domain_os_set_machine(GVirConfigDomainOs *os, const char *machine);
void gvir_config_domain_os_set_smbios_mode(GVirConfigDomainOs *os,
                                           GVirConfigDomainOsSmBiosMode mode);
void gvir_config_domain_os_enable_boot_menu(GVirConfigDomainOs *os, gboolean enable);
void gvir_config_domain_os_bios_enable_serial(GVirConfigDomainOs *os, gboolean enable);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_OS_H__ */
