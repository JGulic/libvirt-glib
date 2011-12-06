/*
 * libvirt-gconfig-domain-chardev-source.c: libvirt domain chardev source base class
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

#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_CHARDEV_SOURCE, GVirConfigDomainChardevSourcePrivate))

struct _GVirConfigDomainChardevSourcePrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainChardevSource, gvir_config_domain_chardev_source, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_domain_chardev_source_class_init(GVirConfigDomainChardevSourceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainChardevSourcePrivate));
}


static void gvir_config_domain_chardev_source_init(GVirConfigDomainChardevSource *source)
{
    g_debug("Init GVirConfigDomainChardevSource=%p", source);

    source->priv = GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_GET_PRIVATE(source);
}
