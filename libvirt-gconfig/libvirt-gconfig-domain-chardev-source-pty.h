/*
 * libvirt-gconfig-domain-chardev-source-pty.h: libvirt domain chardev pty configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_PTY_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_PTY_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY            (gvir_config_domain_chardev_source_pty_get_type ())
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY, GVirConfigDomainChardevSourcePty))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY, GVirConfigDomainChardevSourcePtyClass))
#define GVIR_IS_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY))
#define GVIR_IS_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY, GVirConfigDomainChardevSourcePtyClass))

typedef struct _GVirConfigDomainChardevSourcePty GVirConfigDomainChardevSourcePty;
typedef struct _GVirConfigDomainChardevSourcePtyPrivate GVirConfigDomainChardevSourcePtyPrivate;
typedef struct _GVirConfigDomainChardevSourcePtyClass GVirConfigDomainChardevSourcePtyClass;

struct _GVirConfigDomainChardevSourcePty
{
    GVirConfigDomainChardevSource parent;

    GVirConfigDomainChardevSourcePtyPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainChardevSourcePtyClass
{
    GVirConfigDomainChardevSourceClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_chardev_source_pty_get_type(void);

GVirConfigDomainChardevSourcePty *gvir_config_domain_chardev_source_pty_new(void);
GVirConfigDomainChardevSourcePty *gvir_config_domain_chardev_source_pty_new_from_xml(const gchar *xml,
                                                                              GError **error);
void gvir_config_domain_source_pty_set_path(GVirConfigDomainChardevSourcePty *pty,
                                            const char *path);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_PTY_H__ */
