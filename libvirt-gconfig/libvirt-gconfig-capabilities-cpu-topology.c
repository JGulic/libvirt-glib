/*
 * libvirt-gconfig-capabilities-cpu-topology.c: libvirt CPU topology capabilities
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2012 Red Hat, Inc.
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
 * Authors: Zeeshan Ali <zeenix@redhat.com>
 *          Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_CPU_TOPOLOGY_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_TOPOLOGY, GVirConfigCapabilitiesCpuTopologyPrivate))

struct _GVirConfigCapabilitiesCpuTopologyPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigCapabilitiesCpuTopology, gvir_config_capabilities_cpu_topology, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_capabilities_cpu_topology_class_init(GVirConfigCapabilitiesCpuTopologyClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigCapabilitiesCpuTopologyPrivate));
}

static void gvir_config_capabilities_cpu_topology_init(GVirConfigCapabilitiesCpuTopology *topology)
{
    g_debug("Init GVirConfigCapabilitiesCpuTopology=%p", topology);

    topology->priv = GVIR_CONFIG_CAPABILITIES_CPU_TOPOLOGY_GET_PRIVATE(topology);
}

guint64
gvir_config_capabilities_cpu_topology_get_sockets(GVirConfigCapabilitiesCpuTopology *topology)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_TOPOLOGY(topology), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(topology),
                                                   NULL,
                                                   "sockets",
                                                   0);
}

guint64
gvir_config_capabilities_cpu_topology_get_cores(GVirConfigCapabilitiesCpuTopology *topology)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_TOPOLOGY(topology), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(topology),
                                                   NULL,
                                                   "cores",
                                                   0);
}

guint64
gvir_config_capabilities_cpu_topology_get_threads(GVirConfigCapabilitiesCpuTopology *topology)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_TOPOLOGY(topology), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(topology),
                                                   NULL,
                                                   "threads",
                                                   0);
}
