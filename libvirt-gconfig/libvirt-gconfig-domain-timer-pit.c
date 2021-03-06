/*
 * libvirt-gconfig-domain-timer-pit.c: libvirt domain PIT timer configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_TIMER_PIT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT, GVirConfigDomainTimerPitPrivate))

struct _GVirConfigDomainTimerPitPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainTimerPit, gvir_config_domain_timer_pit, GVIR_CONFIG_TYPE_DOMAIN_TIMER);


static void gvir_config_domain_timer_pit_class_init(GVirConfigDomainTimerPitClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainTimerPitPrivate));
}


static void gvir_config_domain_timer_pit_init(GVirConfigDomainTimerPit *timer)
{
    g_debug("Init GVirConfigDomainTimerPit=%p", timer);

    timer->priv = GVIR_CONFIG_DOMAIN_TIMER_PIT_GET_PRIVATE(timer);
}


GVirConfigDomainTimerPit *gvir_config_domain_timer_pit_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT,
                                    "timer", NULL);
    gvir_config_object_set_attribute(object, "name", "pit", NULL);
    return GVIR_CONFIG_DOMAIN_TIMER_PIT(object);
}

GVirConfigDomainTimerPit *gvir_config_domain_timer_pit_new_from_xml(const gchar *xml,
                                                GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT,
                                             "timer", NULL, xml, error);
    gvir_config_object_set_attribute(object, "name", "pit", NULL);
    return GVIR_CONFIG_DOMAIN_TIMER_PIT(object);
}
