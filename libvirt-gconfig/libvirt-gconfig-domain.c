/*
 * libvirt-gobject-config_domain.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010 Red Hat
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

#include <string.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN, GVirConfigDomainPrivate))

struct _GVirConfigDomainPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomain, gvir_config_domain, GVIR_TYPE_CONFIG_OBJECT);

enum {
    PROP_0,
    PROP_NAME,
    PROP_MEMORY,
    PROP_VCPU,
    PROP_FEATURES
};

static void gvir_config_domain_get_property(GObject *object,
                                            guint prop_id,
                                            GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigDomain *domain = GVIR_CONFIG_DOMAIN(object);

    switch (prop_id) {
    case PROP_NAME:
        g_value_take_string(value, gvir_config_domain_get_name(domain));
        break;
    case PROP_MEMORY:
        g_value_set_uint64(value, gvir_config_domain_get_memory(domain));
        break;
    case PROP_VCPU:
        g_value_set_uint64(value, gvir_config_domain_get_vcpus(domain));
        break;
    case PROP_FEATURES:
        g_value_take_boxed(value, gvir_config_domain_get_features(domain));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_config_domain_set_property(GObject *object,
                                            guint prop_id,
                                            const GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigDomain *domain = GVIR_CONFIG_DOMAIN(object);

    switch (prop_id) {
    case PROP_NAME:
        gvir_config_domain_set_name(domain, g_value_get_string(value));
        break;
    case PROP_MEMORY:
        gvir_config_domain_set_memory(domain, g_value_get_uint64(value));
        break;
    case PROP_VCPU:
        gvir_config_domain_set_vcpus(domain, g_value_get_uint64(value));
        break;
    case PROP_FEATURES:
        gvir_config_domain_set_features(domain, g_value_get_boxed(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_config_domain_class_init(GVirConfigDomainClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(GVirConfigDomainPrivate));

    object_class->get_property = gvir_config_domain_get_property;
    object_class->set_property = gvir_config_domain_set_property;

    g_object_class_install_property(object_class,
                                    PROP_NAME,
                                    g_param_spec_string("name",
                                                        "Name",
                                                        "Domain Name",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_MEMORY,
                                    g_param_spec_uint64("memory",
                                                        "Memory",
                                                        "Maximum Guest Memory (in kilobytes)",
                                                        0, G_MAXUINT64,
                                                        0,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_VCPU,
                                    g_param_spec_uint64("vcpu",
                                                        "Virtual CPUs",
                                                        "Maximum Number of Guest Virtual CPUs",
                                                        0, G_MAXUINT64,
                                                        1,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_FEATURES,
                                    g_param_spec_boxed("features",
                                                        "Features",
                                                        "Hypervisor Features",
                                                        G_TYPE_STRV,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
}


static void gvir_config_domain_init(GVirConfigDomain *conn)
{
    g_debug("Init GVirConfigDomain=%p", conn);

    conn->priv = GVIR_CONFIG_DOMAIN_GET_PRIVATE(conn);
}


GVirConfigDomain *gvir_config_domain_new_from_xml(const gchar *xml,
                                                  GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN,
                                             "domain",
                                             DATADIR "/libvirt/schemas/domain.rng",
                                             xml, error);
    return GVIR_CONFIG_DOMAIN(object);
}

GVirConfigDomain *gvir_config_domain_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN,
                                    "domain",
                                    DATADIR "/libvirt/schemas/domain.rng");
    return GVIR_CONFIG_DOMAIN(object);
}

char *gvir_config_domain_get_name(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(domain),
                                               "name");
}

void gvir_config_domain_set_name(GVirConfigDomain *domain, const char *name)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(domain),
                                        "name", name);
    g_object_notify(G_OBJECT(domain), "name");
}

guint64 gvir_config_domain_get_memory(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                                      "memory");
}

void gvir_config_domain_set_memory(GVirConfigDomain *domain, guint64 memory)
{
    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                               "memory", memory);
    g_object_notify(G_OBJECT(domain), "memory");
}

guint64 gvir_config_domain_get_vcpus(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                                      "vcpu");
}

void gvir_config_domain_set_vcpus(GVirConfigDomain *domain, guint64 vcpu_count)
{
    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                               "vcpu", vcpu_count);
    g_object_notify(G_OBJECT(domain), "vcpu");
}

/**
 * gvir_config_domain_get_features:
 * Returns: (transfer full):
 */
GStrv gvir_config_domain_get_features(GVirConfigDomain *domain)
{
    GPtrArray *features;
    xmlNodePtr parent_node;
    xmlNodePtr node;
    xmlNodePtr it;

    parent_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(domain));
    if (parent_node == NULL)
        return NULL;

    node = gvir_config_xml_get_element(parent_node, "features", NULL);
    if (node == NULL)
        return NULL;

    features = g_ptr_array_new();
    for (it = node->children; it != NULL; it = it->next) {
        g_ptr_array_add(features, g_strdup((char *)it->name));
    }
    g_ptr_array_add(features, NULL);

    return (GStrv)g_ptr_array_free(features, FALSE);
}

void gvir_config_domain_set_features(GVirConfigDomain *domain,
                                     const GStrv features)
{
    xmlNodePtr features_node;
    GStrv it;

    features_node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(domain),
                                                     "features");
    for (it = features; *it != NULL; it++) {
        xmlNodePtr node;

        node = xmlNewDocNode(features_node->doc, NULL, (xmlChar *)*it, NULL);
        xmlAddChild(features_node, node);
    }
    g_object_notify(G_OBJECT(domain), "features");
}

void gvir_config_domain_set_clock(GVirConfigDomain *domain,
                                  GVirConfigDomainClock *klock)
{
    xmlNodePtr clock_node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN(domain));
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_CLOCK(klock));

    clock_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(klock));
    gvir_config_object_set_child(GVIR_CONFIG_OBJECT(domain), clock_node);
}

void gvir_config_domain_set_os(GVirConfigDomain *domain,
                               GVirConfigDomainOs *os)
{
    xmlNodePtr os_node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN(domain));
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

    os_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(os));
    gvir_config_object_set_child(GVIR_CONFIG_OBJECT(domain), os_node);
}

/**
 * gvir_config_domain_set_devices:
 * @devices: (in) (element-type LibvirtGConfig.DomainDevice):
 */
void gvir_config_domain_set_devices(GVirConfigDomain *domain,
                                    GList *devices)
{
    xmlNodePtr devices_node;
    GList *it;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN(domain));

    devices_node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(domain),
                                                    "devices");
    for (it = devices; it != NULL; it = it->next) {
        GVirConfigDomainDevice *device = GVIR_CONFIG_DOMAIN_DEVICE(it->data);
        xmlNodePtr node;

        node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(device));
        xmlAddChild(devices_node, node);
    }
}
