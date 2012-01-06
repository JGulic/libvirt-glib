/*
 * libvirt-gconfig-domain-device-private.h: libvirt domain device configuration
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_DEVICE_PRIVATE_H__
#define __LIBVIRT_GCONFIG_DOMAIN_DEVICE_PRIVATE_H__

#include <libvirt-gconfig/libvirt-gconfig-xml-doc.h>

G_BEGIN_DECLS

GVirConfigDomainDevice *
gvir_config_domain_device_new_from_tree(GVirConfigXmlDoc *doc,
                                        xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_disk_new_from_tree(GVirConfigXmlDoc *doc,
                                      xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_graphics_new_from_tree(GVirConfigXmlDoc *doc,
                                          xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_interface_new_from_tree(GVirConfigXmlDoc *doc,
                                           xmlNodePtr tree);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_H__ */
