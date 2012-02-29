/*
 * libvirt-gconfig-domain-interface.h: libvirt domain interface configuration
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_H__
#define __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_INTERFACE            (gvir_config_domain_interface_get_type ())
#define GVIR_CONFIG_DOMAIN_INTERFACE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterface))
#define GVIR_CONFIG_DOMAIN_INTERFACE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceClass))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_DOMAIN_INTERFACE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceClass))

typedef struct _GVirConfigDomainInterface GVirConfigDomainInterface;
typedef struct _GVirConfigDomainInterfacePrivate GVirConfigDomainInterfacePrivate;
typedef struct _GVirConfigDomainInterfaceClass GVirConfigDomainInterfaceClass;

struct _GVirConfigDomainInterface
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainInterfacePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainInterfaceClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_INTERFACE_LINK_STATE_DEFAULT,
    GVIR_CONFIG_DOMAIN_INTERFACE_LINK_STATE_UP,
    GVIR_CONFIG_DOMAIN_INTERFACE_LINK_STATE_DOWN
} GVirConfigDomainInterfaceLinkState;

GType gvir_config_domain_interface_get_type(void);

void gvir_config_domain_interface_set_ifname(GVirConfigDomainInterface *interface,
                                             const char *ifname);
void gvir_config_domain_interface_set_link_state(GVirConfigDomainInterface *interface,
                                                 GVirConfigDomainInterfaceLinkState state);
void gvir_config_domain_interface_set_mac(GVirConfigDomainInterface *interface,
                                          const char *mac_address);
void gvir_config_domain_interface_set_model(GVirConfigDomainInterface *interface,
                                            const char *model);
char *gvir_config_domain_interface_get_ifname(GVirConfigDomainInterface *interface);
GVirConfigDomainInterfaceLinkState gvir_config_domain_interface_get_link_state(GVirConfigDomainInterface *interface);
char *gvir_config_domain_interface_get_mac(GVirConfigDomainInterface *interface);
char *gvir_config_domain_interface_get_model(GVirConfigDomainInterface *interface);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_H__ */
