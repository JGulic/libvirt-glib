/*
 * libvirt-gconfig-domain-clock.h: libvirt domain clock configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CLOCK_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CLOCK_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_DOMAIN_CLOCK            (gvir_config_domain_clock_get_type ())
#define GVIR_CONFIG_DOMAIN_CLOCK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_DOMAIN_CLOCK, GVirConfigDomainClock))
#define GVIR_CONFIG_DOMAIN_CLOCK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_DOMAIN_CLOCK, GVirConfigDomainClockClass))
#define GVIR_IS_CONFIG_DOMAIN_CLOCK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_DOMAIN_CLOCK))
#define GVIR_IS_CONFIG_DOMAIN_CLOCK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_DOMAIN_CLOCK))
#define GVIR_CONFIG_DOMAIN_CLOCK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_DOMAIN_CLOCK, GVirConfigDomainClockClass))

typedef struct _GVirConfigDomainClock GVirConfigDomainClock;
typedef struct _GVirConfigDomainClockPrivate GVirConfigDomainClockPrivate;
typedef struct _GVirConfigDomainClockClass GVirConfigDomainClockClass;

struct _GVirConfigDomainClock
{
    GVirConfigObject parent;

    GVirConfigDomainClockPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainClockClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_CLOCK_UTC,
    GVIR_CONFIG_DOMAIN_CLOCK_LOCALTIME,
    GVIR_CONFIG_DOMAIN_CLOCK_TIMEZONE,
    GVIR_CONFIG_DOMAIN_CLOCK_VARIABLE
} GVirConfigDomainClockOffset;

GType gvir_config_domain_clock_get_type(void);

GVirConfigDomainClock *gvir_config_domain_clock_new(void);
GVirConfigDomainClock *gvir_config_domain_clock_new_from_xml(const gchar *xml,
                                                GError **error);
void gvir_config_domain_clock_set_offset(GVirConfigDomainClock *klock,
                                         GVirConfigDomainClockOffset offset);
void gvir_config_domain_clock_set_timezone(GVirConfigDomainClock *klock,
                                           const char *tz);
void gvir_config_domain_clock_set_variable_offset(GVirConfigDomainClock *klock,
                                                  gint seconds);


G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CLOCK_H__ */
