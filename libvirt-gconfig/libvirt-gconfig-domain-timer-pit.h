/*
 * libvirt-gconfig-domain-timer-pit.h: libvirt domain PIT timer configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_TIMER_PIT_H__
#define __LIBVIRT_GCONFIG_DOMAIN_TIMER_PIT_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT            (gvir_config_domain_timer_pit_get_type ())
#define GVIR_CONFIG_DOMAIN_TIMER_PIT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT, GVirConfigDomainTimerPit))
#define GVIR_CONFIG_DOMAIN_TIMER_PIT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT, GVirConfigDomainTimerPitClass))
#define GVIR_CONFIG_IS_DOMAIN_TIMER_PIT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT))
#define GVIR_CONFIG_IS_DOMAIN_TIMER_PIT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT))
#define GVIR_CONFIG_DOMAIN_TIMER_PIT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT, GVirConfigDomainTimerPitClass))

typedef struct _GVirConfigDomainTimerPit GVirConfigDomainTimerPit;
typedef struct _GVirConfigDomainTimerPitPrivate GVirConfigDomainTimerPitPrivate;
typedef struct _GVirConfigDomainTimerPitClass GVirConfigDomainTimerPitClass;

struct _GVirConfigDomainTimerPit
{
    GVirConfigObject parent;

    GVirConfigDomainTimerPitPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainTimerPitClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_timer_pit_get_type(void);

GVirConfigDomainTimerPit *gvir_config_domain_timer_pit_new(void);
GVirConfigDomainTimerPit *gvir_config_domain_timer_pit_new_from_xml(const gchar *xml,
                                                                    GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_TIMER_PIT_H__ */
