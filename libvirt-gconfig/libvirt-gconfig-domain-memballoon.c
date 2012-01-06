/*
 * libvirt-gconfig-domain-memballoon.c: libvirt domain memballoon configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_MEMBALLOON_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_MEMBALLOON, GVirConfigDomainMemballoonPrivate))

struct _GVirConfigDomainMemballoonPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainMemballoon, gvir_config_domain_memballoon, GVIR_TYPE_CONFIG_DOMAIN_DEVICE);


static void gvir_config_domain_memballoon_class_init(GVirConfigDomainMemballoonClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainMemballoonPrivate));
}


static void gvir_config_domain_memballoon_init(GVirConfigDomainMemballoon *memballoon)
{
    g_debug("Init GVirConfigDomainMemballoon=%p", memballoon);

    memballoon->priv = GVIR_CONFIG_DOMAIN_MEMBALLOON_GET_PRIVATE(memballoon);
}


GVirConfigDomainMemballoon *gvir_config_domain_memballoon_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_MEMBALLOON,
                                    "memballoon", NULL);
    return GVIR_CONFIG_DOMAIN_MEMBALLOON(object);
}

GVirConfigDomainMemballoon *gvir_config_domain_memballoon_new_from_xml(const gchar *xml,
                                                                       GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_MEMBALLOON,
                                             "memballoon", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_MEMBALLOON(object);
}

void gvir_config_domain_memballoon_set_model(GVirConfigDomainMemballoon *memballoon,
                                             GVirConfigDomainMemballoonModel model)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_MEMBALLOON(memballoon));
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(memballoon),
                                               "model",
                                               GVIR_TYPE_CONFIG_DOMAIN_MEMBALLOON_MODEL,
                                               model,
                                               NULL);
}
