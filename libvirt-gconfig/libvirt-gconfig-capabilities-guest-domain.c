/*
 * libvirt-gconfig-capabilities-guest-domain.c: libvirt guest domain capabilities
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2012 Red Hat, Inc.
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
 * Authors: Zeeshan Ali <zeenix@redhat.com>
 *          Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_GUEST_DOMAIN_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN, GVirConfigCapabilitiesGuestDomainPrivate))

struct _GVirConfigCapabilitiesGuestDomainPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigCapabilitiesGuestDomain, gvir_config_capabilities_guest_domain, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_capabilities_guest_domain_class_init(GVirConfigCapabilitiesGuestDomainClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigCapabilitiesGuestDomainPrivate));
}

static void gvir_config_capabilities_guest_domain_init(GVirConfigCapabilitiesGuestDomain *domain)
{
    g_debug("Init GVirConfigCapabilitiesGuestDomain=%p", domain);

    domain->priv = GVIR_CONFIG_CAPABILITIES_GUEST_DOMAIN_GET_PRIVATE(domain);
}

const gchar *
gvir_config_capabilities_guest_domain_get_emulator(GVirConfigCapabilitiesGuestDomain *domain)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST_DOMAIN(domain), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(domain), "emulator");
}

GVirConfigDomainVirtType
gvir_config_capabilities_guest_domain_get_virt_type(GVirConfigCapabilitiesGuestDomain *domain)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST_DOMAIN(domain),
                         GVIR_CONFIG_DOMAIN_VIRT_QEMU);

    return gvir_config_object_get_attribute_genum
                                (GVIR_CONFIG_OBJECT(domain),
                                 NULL,
                                 "type",
                                 GVIR_CONFIG_TYPE_DOMAIN_VIRT_TYPE,
                                 GVIR_CONFIG_DOMAIN_VIRT_QEMU);
}
