/*
 * libvirt-gobject-domain.c: libvirt gobject integration
 *
 * Copyright (C) 2010 Red Hat
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

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_DOMAIN_H__
#define __LIBVIRT_GOBJECT_DOMAIN_H__

G_BEGIN_DECLS

#define GVIR_TYPE_DOMAIN            (gvir_domain_get_type ())
#define GVIR_DOMAIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_DOMAIN, GVirDomain))
#define GVIR_DOMAIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_DOMAIN, GVirDomainClass))
#define GVIR_IS_DOMAIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_DOMAIN))
#define GVIR_IS_DOMAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_DOMAIN))
#define GVIR_DOMAIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_DOMAIN, GVirDomainClass))

#define GVIR_TYPE_DOMAIN_INFO       (gvir_domain_info_get_type())
#define GVIR_TYPE_DOMAIN_HANDLE     (gvir_domain_handle_get_type())

typedef struct _GVirDomain GVirDomain;
typedef struct _GVirDomainPrivate GVirDomainPrivate;
typedef struct _GVirDomainClass GVirDomainClass;

struct _GVirDomain
{
    GObject parent;

    GVirDomainPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirDomainClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};


typedef enum {
    GVIR_DOMAIN_STATE_NONE    = 0, /* no state */
    GVIR_DOMAIN_STATE_RUNNING = 1, /* the domain is running */
    GVIR_DOMAIN_STATE_BLOCKED = 2, /* the domain is blocked on resource */
    GVIR_DOMAIN_STATE_PAUSED  = 3, /* the domain is paused by user */
    GVIR_DOMAIN_STATE_SHUTDOWN= 4, /* the domain is being shut down */
    GVIR_DOMAIN_STATE_SHUTOFF = 5, /* the domain is shut off */
    GVIR_DOMAIN_STATE_CRASHED = 6  /* the domain is crashed */
} GVirDomainState;


typedef struct _GVirDomainInfo GVirDomainInfo;
struct _GVirDomainInfo
{
    GVirDomainState state; /* the running state */
    guint64 maxMem;        /* the maximum memory in KBytes allowed */
    guint64 memory;        /* the memory in KBytes used by the domain */
    guint16 nrVirtCpu;     /* the number of virtual CPUs for the domain */
    guint64 cpuTime;       /* the CPU time used in nanoseconds */
};

GType gvir_domain_get_type(void);
GType gvir_domain_info_get_type(void);
GType gvir_domain_handle_get_type(void);

gchar *gvir_domain_get_name(GVirDomain *dom);
gchar *gvir_domain_get_uuid(GVirDomain *dom);
gint gvir_domain_get_id(GVirDomain *dom,
                        GError **err);

gboolean gvir_domain_start(GVirDomain *dom,
                           GError **err);
gboolean gvir_domain_stop(GVirDomain *dom,
                          GError **err);
gboolean gvir_domain_delete(GVirDomain *dom,
                            GError **err);
gboolean gvir_domain_shutdown(GVirDomain *dom,
                              GError **err);
gboolean gvir_domain_reboot(GVirDomain *dom,
                            GError **err);

GVirDomainInfo *gvir_domain_get_info(GVirDomain *dom,
                                     GError **err);

GVirDomainConfig *gvir_domain_get_config(GVirDomain *dom,
                                         GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_DOMAIN_H__ */
