/*
 * libvirt-gconfig-helpers-private.h: libvirt configuration helpers
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_HELPERS_PRIVATE_H__
#define __LIBVIRT_GCONFIG_HELPERS_PRIVATE_H__

G_BEGIN_DECLS

GError *gvir_xml_error_new(GQuark domain, gint code,
                           const gchar *format, ...);
xmlNodePtr gvir_config_xml_parse(const char *xml,
                                 const char *root_node,
                                 GError **err);
xmlNode * gvir_config_xml_get_element (xmlNode *node, ...);
xmlChar * gvir_config_xml_get_child_element_content (xmlNode    *node,
                                                     const char *child_name);
char *gvir_config_xml_get_child_element_content_glib (xmlNode    *node,
                                                      const char *child_name);
const char *gvir_config_genum_get_nick (GType enum_type, gint value);
G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_HELPERS_PRIVATE_H__ */
