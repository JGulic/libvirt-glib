/*
 * libvirt-gconfig-helpers.c: libvirt configuration helpers
 *
 * Copyright (C) 2010, 2011 Red Hat, Inc.
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include <string.h>

#include <libxml/xmlerror.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"

GQuark
gvir_config_object_error_quark(void)
{
    return g_quark_from_static_string("gvir-config-object");
}

static GError *gvir_xml_error_new_literal(GQuark domain,
                                          gint code,
                                          const gchar *message)
{
    xmlErrorPtr xerr = xmlGetLastError();

    if (!xerr)
        return NULL;

    if (message)
        return g_error_new(domain,
                           code,
                           "%s: %s",
                           message,
                           xerr->message);
    else
        return g_error_new(domain,
                           code,
                           "%s",
                           xerr->message);
}


GError *gvir_xml_error_new(GQuark domain,
                           gint code,
                           const gchar *format,
                           ...)
{
    GError *err;
    va_list args;
    gchar *message;

    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);

    err = gvir_xml_error_new_literal(domain, code, message);

    g_free(message);

    return err;
}

xmlNodePtr
gvir_config_xml_parse(const char *xml, const char *root_node, GError **err)
{
    xmlDocPtr doc;

    if (!xml) {
        *err = g_error_new(GVIR_CONFIG_OBJECT_ERROR,
                           0,
                           "%s",
                           "No XML document to parse");
        return NULL;
    }

    doc = xmlParseMemory(xml, strlen(xml));
    if (!doc) {
        *err = gvir_xml_error_new(GVIR_CONFIG_OBJECT_ERROR,
                                  0,
                                  "%s",
                                  "Unable to parse configuration");
        return NULL;
    }
    if ((!doc->children) || (strcmp((char *)doc->children->name, root_node) != 0)) {
        *err = g_error_new(GVIR_CONFIG_OBJECT_ERROR,
                           0,
                           "XML data has no '%s' node",
                           root_node);
        xmlFreeDoc(doc);
        return NULL;
    }

    return doc->children;
}


/*
 * gvir_config_xml_get_element, gvir_config_xml_get_child_element_content
 * and gvir_config_xml_get_child_element_content_glib are:
 *
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 */
xmlNode *
gvir_config_xml_get_element (xmlNode *node, ...)
{
        va_list var_args;

        va_start (var_args, node);

        while (TRUE) {
                const char *arg;

                arg = va_arg (var_args, const char *);
                if (!arg)
                        break;

                for (node = node->children; node; node = node->next)
                        if (!g_strcmp0 (arg, (char *) node->name))
                                break;

                if (!node)
                        break;
        }

        va_end (var_args);

        return node;
}

xmlChar *
gvir_config_xml_get_child_element_content (xmlNode    *node,
                                           const char *child_name)
{
        xmlNode *child_node;

        child_node = gvir_config_xml_get_element (node, child_name, NULL);
        if (!child_node)
                return NULL;

        return xmlNodeGetContent (child_node);
}

char *
gvir_config_xml_get_child_element_content_glib (xmlNode    *node,
                                                const char *child_name)
{
        xmlChar *content;
        char *copy;

        content = gvir_config_xml_get_child_element_content (node, child_name);
        if (!content)
                return NULL;

        copy = g_strdup ((char *) content);

        xmlFree (content);

        return copy;
}

const char *gvir_config_genum_get_nick (GType enum_type, gint value)
{
    GEnumClass *enum_class;
    GEnumValue *enum_value;

    g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

    enum_class = g_type_class_ref(enum_type);
    enum_value = g_enum_get_value(enum_class, value);
    g_type_class_unref(enum_class);

    if (enum_value != NULL)
        return enum_value->value_nick;

    g_return_val_if_reached(NULL);
}
