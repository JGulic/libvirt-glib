/*
 * libvirt-gconfig-interface.c: libvirt interface configuration
 *
 * Copyright (C) 2010 Red Hat, Inc.
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

#ifndef __LIBVIRT_GCONFIG_INTERFACE_H__
#define __LIBVIRT_GCONFIG_INTERFACE_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_INTERFACE            (gvir_config_interface_get_type ())
#define GVIR_CONFIG_INTERFACE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_INTERFACE, GVirConfigInterface))
#define GVIR_CONFIG_INTERFACE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_INTERFACE, GVirConfigInterfaceClass))
#define GVIR_IS_CONFIG_INTERFACE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_INTERFACE))
#define GVIR_IS_CONFIG_INTERFACE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_INTERFACE))
#define GVIR_CONFIG_INTERFACE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_INTERFACE, GVirConfigInterfaceClass))

typedef struct _GVirConfigInterface GVirConfigInterface;
typedef struct _GVirConfigInterfacePrivate GVirConfigInterfacePrivate;
typedef struct _GVirConfigInterfaceClass GVirConfigInterfaceClass;

struct _GVirConfigInterface
{
    GVirConfigObject parent;

    GVirConfigInterfacePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigInterfaceClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_interface_get_type(void);

GVirConfigInterface *gvir_config_interface_new(void);
GVirConfigInterface *gvir_config_interface_new_from_xml(const gchar *xml,
                                                        GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_INTERFACE_H__ */
