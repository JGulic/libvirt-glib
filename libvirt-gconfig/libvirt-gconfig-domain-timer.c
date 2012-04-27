/*
 * libvirt-gconfig-domain-timer.c: libvirt domain timer configuration
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

#define GVIR_CONFIG_DOMAIN_TIMER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER, GVirConfigDomainTimerPrivate))

struct _GVirConfigDomainTimerPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainTimer, gvir_config_domain_timer, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_timer_class_init(GVirConfigDomainTimerClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainTimerPrivate));
}


static void gvir_config_domain_timer_init(GVirConfigDomainTimer *timer)
{
    g_debug("Init GVirConfigDomainTimer=%p", timer);

    timer->priv = GVIR_CONFIG_DOMAIN_TIMER_GET_PRIVATE(timer);
}

void gvir_config_domain_timer_set_tick_policy(GVirConfigDomainTimer *timer,
                                              GVirConfigDomainTimerTickPolicy policy)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_TIMER(timer));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(timer),
                                               "tickpolicy",
                                               GVIR_CONFIG_TYPE_DOMAIN_TIMER_TICK_POLICY,
                                               policy,
                                               NULL);
}

GVirConfigDomainTimerTickPolicy
gvir_config_domain_timer_get_tick_policy(GVirConfigDomainTimer *timer)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_TIMER(timer),
                         GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(timer),
                                                  NULL,
                                                  "tickpolicy",
                                                  GVIR_CONFIG_TYPE_DOMAIN_TIMER_TICK_POLICY,
                                                  GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);
}
