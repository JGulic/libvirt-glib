/*
 * libvirt-gconfig-domain-chardev.h: libvirt domain chardev configuration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_H__

#include <libvirt-gconfig/libvirt-gconfig-domain-chardev-source.h>

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_DOMAIN_CHARDEV            (gvir_config_domain_chardev_get_type ())
#define GVIR_CONFIG_DOMAIN_CHARDEV(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV, GVirConfigDomainChardev))
#define GVIR_CONFIG_DOMAIN_CHARDEV_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV, GVirConfigDomainChardevClass))
#define GVIR_IS_CONFIG_DOMAIN_CHARDEV(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV))
#define GVIR_IS_CONFIG_DOMAIN_CHARDEV_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV))
#define GVIR_CONFIG_DOMAIN_CHARDEV_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV, GVirConfigDomainChardevClass))

typedef struct _GVirConfigDomainChardev GVirConfigDomainChardev;
typedef struct _GVirConfigDomainChardevPrivate GVirConfigDomainChardevPrivate;
typedef struct _GVirConfigDomainChardevClass GVirConfigDomainChardevClass;

struct _GVirConfigDomainChardev
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainChardevPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainChardevClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_chardev_get_type(void);
void gvir_config_domain_chardev_set_source(GVirConfigDomainChardev *chardev,
                                           GVirConfigDomainChardevSource *source);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_H__ */
