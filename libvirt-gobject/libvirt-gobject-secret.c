/*
 * libvirt-gobject-secret.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2011 Red Hat, Inc.
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

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"

#define GVIR_SECRET_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_SECRET, GVirSecretPrivate))

struct _GVirSecretPrivate
{
    virSecretPtr handle;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
};

G_DEFINE_TYPE(GVirSecret, gvir_secret, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_SECRET_ERROR gvir_secret_error_quark()


static GQuark
gvir_secret_error_quark(void)
{
    return g_quark_from_static_string("gvir-secret");
}

static void gvir_secret_get_property(GObject *object,
                                     guint prop_id,
                                     GValue *value,
                                     GParamSpec *pspec)
{
    GVirSecret *conn = GVIR_SECRET(object);
    GVirSecretPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_secret_set_property(GObject *object,
                                     guint prop_id,
                                     const GValue *value,
                                     GParamSpec *pspec)
{
    GVirSecret *conn = GVIR_SECRET(object);
    GVirSecretPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virSecretFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_secret_finalize(GObject *object)
{
    GVirSecret *conn = GVIR_SECRET(object);
    GVirSecretPrivate *priv = conn->priv;

    g_debug("Finalize GVirSecret=%p", conn);

    virSecretFree(priv->handle);

    G_OBJECT_CLASS(gvir_secret_parent_class)->finalize(object);
}


static void gvir_secret_constructed(GObject *object)
{
    GVirSecret *conn = GVIR_SECRET(object);
    GVirSecretPrivate *priv = conn->priv;

    G_OBJECT_CLASS(gvir_secret_parent_class)->constructed(object);

    /* xxx we may want to turn this into an initable */
    if (virSecretGetUUIDString(priv->handle, priv->uuid) < 0) {
        g_error("Failed to get secret UUID on %p", priv->handle);
    }
}


static void gvir_secret_class_init(GVirSecretClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_secret_finalize;
    object_class->get_property = gvir_secret_get_property;
    object_class->set_property = gvir_secret_set_property;
    object_class->constructed = gvir_secret_constructed;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The secret handle",
                                                       GVIR_TYPE_SECRET_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirSecretPrivate));
}


static void gvir_secret_init(GVirSecret *conn)
{
    g_debug("Init GVirSecret=%p", conn);

    conn->priv = GVIR_SECRET_GET_PRIVATE(conn);
}

typedef struct virSecret GVirSecretHandle;

static GVirSecretHandle*
gvir_secret_handle_copy(GVirSecretHandle *src)
{
    virSecretRef((virSecretPtr)src);
    return src;
}

static void
gvir_secret_handle_free(GVirSecretHandle *src)
{
    virSecretFree((virSecretPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirSecretHandle, gvir_secret_handle,
                    gvir_secret_handle_copy, gvir_secret_handle_free)

const gchar *gvir_secret_get_uuid(GVirSecret *secret)
{
    g_return_val_if_fail(GVIR_IS_SECRET(secret), NULL);

    return secret->priv->uuid;
}


/**
 * gvir_secret_get_config:
 * @secret: the secret
 * @flags: the flags
 * Returns: (transfer full): the config
 */
GVirConfigSecret *gvir_secret_get_config(GVirSecret *secret,
                                         guint flags,
                                         GError **err)
{
    GVirSecretPrivate *priv = secret->priv;
    gchar *xml;

    if (!(xml = virSecretGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_SECRET_ERROR,
                               0,
                               "Unable to get secret XML config");
        return NULL;
    }

    GVirConfigSecret *conf = gvir_config_secret_new_from_xml(xml, err);

    free(xml);
    return conf;
}
