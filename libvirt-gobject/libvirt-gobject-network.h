/*
 * libvirt-gobject-network.c: libvirt gobject integration
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

#ifndef __LIBVIRT_GOBJECT_NETWORK_H__
#define __LIBVIRT_GOBJECT_NETWORK_H__

G_BEGIN_DECLS

#define GVIR_TYPE_NETWORK            (gvir_network_get_type ())
#define GVIR_NETWORK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_NETWORK, GVirNetwork))
#define GVIR_NETWORK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_NETWORK, GVirNetworkClass))
#define GVIR_IS_NETWORK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_NETWORK))
#define GVIR_IS_NETWORK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_NETWORK))
#define GVIR_NETWORK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_NETWORK, GVirNetworkClass))

#define GVIR_TYPE_NETWORK_HANDLE     (gvir_network_handle_get_type())

typedef struct _GVirNetwork GVirNetwork;
typedef struct _GVirNetworkPrivate GVirNetworkPrivate;
typedef struct _GVirNetworkClass GVirNetworkClass;

struct _GVirNetwork
{
    GObject parent;

    GVirNetworkPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirNetworkClass
{
    GObjectClass parent_class;

    void (*vir_network_started)(GVirNetwork *net);
    void (*vir_network_stopped)(GVirNetwork *net);

    gpointer padding[20];
};


GType gvir_network_get_type(void);
GType gvir_network_handle_get_type(void);

const gchar *gvir_network_get_name(GVirNetwork *network);
gchar *gvir_network_get_uuid(GVirNetwork *network);

GVirConfigNetwork *gvir_network_get_config(GVirNetwork *network,
                                           guint64 flags,
                                           GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_NETWORK_H__ */
