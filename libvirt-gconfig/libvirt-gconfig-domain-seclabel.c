/*
 * libvirt-gconfig-domain-seclabel.c: libvirt glib integration
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPSECLABELE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Bseclabelton, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_SECLABEL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_SECLABEL, GVirConfigDomainSeclabelPrivate))

struct _GVirConfigDomainSeclabelPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainSeclabel, gvir_config_domain_seclabel, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_domain_seclabel_class_init(GVirConfigDomainSeclabelClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainSeclabelPrivate));
}


static void gvir_config_domain_seclabel_init(GVirConfigDomainSeclabel *seclabel)
{
    g_debug("Init GVirConfigDomainSeclabel=%p", seclabel);

    seclabel->priv = GVIR_CONFIG_DOMAIN_SECLABEL_GET_PRIVATE(seclabel);
}


GVirConfigDomainSeclabel *gvir_config_domain_seclabel_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_SECLABEL, "seclabel", NULL);
    return GVIR_CONFIG_DOMAIN_SECLABEL(object);
}

GVirConfigDomainSeclabel *gvir_config_domain_seclabel_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_SECLABEL, "seclabel",
                                             NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_SECLABEL(object);
}

void gvir_config_domain_seclabel_set_type(GVirConfigDomainSeclabel *seclabel,
                                          GVirConfigDomainSeclabelType type)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_SECLABEL(seclabel));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(seclabel),
                                               "type",
                                               GVIR_TYPE_CONFIG_DOMAIN_SECLABEL_TYPE,
                                               type, NULL);
}

void gvir_config_domain_seclabel_set_model(GVirConfigDomainSeclabel *seclabel,
                                           const gchar *model)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_SECLABEL(seclabel));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(seclabel),
                                     "model", model,
                                     NULL);

}

void gvir_config_domain_seclabel_set_baselabel(GVirConfigDomainSeclabel *seclabel,
                                               const char *label)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_SECLABEL(seclabel));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(seclabel),
                                        "baselabel", label);
}

void gvir_config_domain_seclabel_set_label(GVirConfigDomainSeclabel *seclabel,
                                           const char *label)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_SECLABEL(seclabel));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(seclabel),
                                        "label", label);
}
