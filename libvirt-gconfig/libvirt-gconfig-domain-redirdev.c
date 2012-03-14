/*
 * libvirt-gconfig-domain-redirdev.c: libvirt domain redirdev configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_REDIRDEV_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV, GVirConfigDomainRedirdevPrivate))

struct _GVirConfigDomainRedirdevPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainRedirdev, gvir_config_domain_redirdev, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV);


static void gvir_config_domain_redirdev_class_init(GVirConfigDomainRedirdevClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainRedirdevPrivate));
}


static void gvir_config_domain_redirdev_init(GVirConfigDomainRedirdev *redirdev)
{
    g_debug("Init GVirConfigDomainRedirdev=%p", redirdev);

    redirdev->priv = GVIR_CONFIG_DOMAIN_REDIRDEV_GET_PRIVATE(redirdev);
}


GVirConfigDomainRedirdev *gvir_config_domain_redirdev_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV,
                                    "redirdev", NULL);
    return GVIR_CONFIG_DOMAIN_REDIRDEV(object);
}

GVirConfigDomainRedirdev *gvir_config_domain_redirdev_new_from_xml(const gchar *xml,
                                                                       GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV,
                                             "redirdev", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_REDIRDEV(object);
}

void gvir_config_domain_redirdev_set_bus(GVirConfigDomainRedirdev *redirdev,
                                         GVirConfigDomainRedirdevBus bus)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_REDIRDEV(redirdev));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(redirdev),
                                               "bus",
                                               GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV_BUS,
                                               bus,
                                               NULL);
}
