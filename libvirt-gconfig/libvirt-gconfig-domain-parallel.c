/*
 * libvirt-gconfig-domain-parallel.c: libvirt domain parallel configuration
 *
 * Copyright (C) 2011-2012 Red Hat, Inc.
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

#define GVIR_CONFIG_DOMAIN_PARALLEL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_PARALLEL, GVirConfigDomainParallelPrivate))

struct _GVirConfigDomainParallelPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainParallel, gvir_config_domain_parallel, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV);


static void gvir_config_domain_parallel_class_init(GVirConfigDomainParallelClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainParallelPrivate));
}


static void gvir_config_domain_parallel_init(GVirConfigDomainParallel *parallel)
{
    g_debug("Init GVirConfigDomainParallel=%p", parallel);

    parallel->priv = GVIR_CONFIG_DOMAIN_PARALLEL_GET_PRIVATE(parallel);
}

GVirConfigDomainParallel *gvir_config_domain_parallel_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_PARALLEL,
                                    "parallel", NULL);
    return GVIR_CONFIG_DOMAIN_PARALLEL(object);
}

GVirConfigDomainParallel *gvir_config_domain_parallel_new_from_xml(const gchar *xml,
                                                                   GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_PARALLEL,
                                             "parallel", NULL, xml, error);
    if (object == NULL)
        return NULL;
    return GVIR_CONFIG_DOMAIN_PARALLEL(object);
}
