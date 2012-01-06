/*
 * libvirt-gconfig-domain-graphics.c: libvirt domain graphics configuration
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS, GVirConfigDomainGraphicsPrivate))

struct _GVirConfigDomainGraphicsPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainGraphics, gvir_config_domain_graphics, GVIR_TYPE_CONFIG_DOMAIN_DEVICE);


static void gvir_config_domain_graphics_class_init(GVirConfigDomainGraphicsClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainGraphicsPrivate));
}


static void gvir_config_domain_graphics_init(GVirConfigDomainGraphics *graphics)
{
    g_debug("Init GVirConfigDomainGraphics=%p", graphics);

    graphics->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_GET_PRIVATE(graphics);
}

G_GNUC_INTERNAL GVirConfigDomainDevice *
gvir_config_domain_graphics_new_from_tree(GVirConfigXmlDoc *doc,
                                          xmlNodePtr tree)
{
    xmlChar *type;
    GType gtype;

    type = gvir_config_xml_get_attribute_content(tree, "type");
    if (type == NULL)
        return NULL;

    if (xmlStrEqual(type, (xmlChar*)"sdl")) {
        gtype = GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SDL;
    } else if (xmlStrEqual(type, (xmlChar*)"vnc")) {
        gtype = GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_VNC;
    } else if (xmlStrEqual(type, (xmlChar*)"spice")) {
        gtype = GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SPICE;
    } else if (xmlStrEqual(type, (xmlChar*)"rdp")) {
        goto unimplemented;
    } else if (xmlStrEqual(type, (xmlChar*)"desktop")) {
        goto unimplemented;
    } else {
        g_debug("Unknown graphics node: %s", type);
        return NULL;
    }
    xmlFree(type);

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(gtype, doc, NULL, tree));

unimplemented:
    g_debug("Parsing of '%s' graphics nodes is unimplemented", type);
    return NULL;
}
