/*
 * libvirt-gobject-connection.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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

#include <config.h>

#include <libvirt/libvirt.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONNECTION_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONNECTION, GVirConnectionPrivate))

struct _GVirConnectionPrivate
{
    GMutex *lock;
    gchar *uri;
    virConnectPtr conn;

    GHashTable *domains;
};

G_DEFINE_TYPE(GVirConnection, gvir_connection, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_URI,
    PROP_HANDLE,
};


#define GVIR_CONNECTION_ERROR gvir_connection_error_quark()

static GQuark
gvir_connection_error_quark(void)
{
    return g_quark_from_static_string("gvir-connection");
}

static void gvir_connection_get_property(GObject *object,
                                         guint prop_id,
                                         GValue *value,
                                         GParamSpec *pspec)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_URI:
        g_value_set_string(value, priv->uri);
        break;
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->conn);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_connection_set_property(GObject *object,
                                         guint prop_id,
                                         const GValue *value,
                                         GParamSpec *pspec)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_URI:
        g_free(priv->uri);
        priv->uri = g_value_dup_string(value);
        break;

    case PROP_HANDLE:
        if (priv->conn)
            virConnectClose(priv->conn);
        priv->conn = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_connection_finalize(GObject *object)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;

    DEBUG("Finalize GVirConnection=%p", conn);

    if (gvir_connection_is_open(conn))
        gvir_connection_close(conn);

    g_mutex_free(priv->lock);
    g_free(priv->uri);

    G_OBJECT_CLASS(gvir_connection_parent_class)->finalize(object);
}


static GVirStream* gvir_connection_stream_new(GVirConnection *self G_GNUC_UNUSED,
                                             gpointer handle)
{
    return g_object_new(GVIR_TYPE_STREAM, "handle", handle, NULL);
}

static void gvir_connection_class_init(GVirConnectionClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_connection_finalize;
    object_class->get_property = gvir_connection_get_property;
    object_class->set_property = gvir_connection_set_property;

    klass->stream_new = gvir_connection_stream_new;

    g_object_class_install_property(object_class,
                                    PROP_URI,
                                    g_param_spec_string("uri",
                                                        "URI",
                                                        "The connection URI",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_signal_new("vir-connection-opened",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, vir_connection_opened),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__VOID,
                 G_TYPE_NONE,
                 0);
    g_signal_new("vir-connection-closed",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, vir_connection_closed),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__VOID,
                 G_TYPE_NONE,
                 0);

    g_signal_new("vir-domain-added",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, vir_domain_added),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__VOID,
                 G_TYPE_NONE,
                 0);
    g_signal_new("vir-domain-removed",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, vir_domain_removed),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__VOID,
                 G_TYPE_NONE,
                 0);

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The connection handle",
                                                       GVIR_TYPE_CONNECTION_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirConnectionPrivate));
}


static void gvir_connection_init(GVirConnection *conn)
{
    GVirConnectionPrivate *priv;

    DEBUG("Init GVirConnection=%p", conn);

    priv = conn->priv = GVIR_CONNECTION_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));

    priv->lock = g_mutex_new();
}


GVirConnection *gvir_connection_new(const char *uri)
{
    return GVIR_CONNECTION(g_object_new(GVIR_TYPE_CONNECTION,
                                         "uri", uri,
                                         NULL));
}


/**
 * gvir_connection_open:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 */
gboolean gvir_connection_open(GVirConnection *conn,
                              GCancellable *cancellable,
                              GError **err)
{
    GVirConnectionPrivate *priv = conn->priv;

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        return FALSE;

    g_mutex_lock(priv->lock);
    if (priv->conn) {
        *err = g_error_new(GVIR_CONNECTION_ERROR,
                           0,
                           "Connection %s is already open",
                           priv->uri);
        g_mutex_unlock(priv->lock);
        return FALSE;
    }

    if (!(priv->conn = virConnectOpen(priv->uri))) {
        *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                              0,
                              "Unable to open %s",
                              priv->uri);
        g_mutex_unlock(priv->lock);
        return FALSE;
    }

    g_mutex_unlock(priv->lock);

    g_signal_emit_by_name(conn, "vir-connection-opened");
    return TRUE;
}


static void
gvir_connection_open_helper(GSimpleAsyncResult *res,
                            GObject *object,
                            GCancellable *cancellable)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GError *err = NULL;

    if (!gvir_connection_open(conn, cancellable, &err)) {
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}


/**
 * gvir_connection_open_async:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (transfer none): completion callback
 * @opaque: (transfer none)(allow-none): opaque data for callback
 */
void gvir_connection_open_async(GVirConnection *conn,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer opaque)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(conn),
                                    callback,
                                    opaque,
                                    gvir_connection_open);
    g_simple_async_result_run_in_thread(res,
                                        gvir_connection_open_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}


/**
 * gvir_connection_open_finish:
 * @conn: the connection
 * @result: (transfer none): async method result
 */
gboolean gvir_connection_open_finish(GVirConnection *conn,
                                     GAsyncResult *result,
                                     GError **err)
{
    g_return_val_if_fail(GVIR_IS_CONNECTION(conn), FALSE);
    g_return_val_if_fail(G_IS_ASYNC_RESULT(result), FALSE);

    if (G_IS_SIMPLE_ASYNC_RESULT(result)) {
        GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT(result);
        g_warn_if_fail (g_simple_async_result_get_source_tag(simple) == gvir_connection_open);
        if (g_simple_async_result_propagate_error(simple, err))
            return FALSE;
    }

    return TRUE;
}


gboolean gvir_connection_is_open(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    gboolean open = TRUE;
    g_mutex_lock(priv->lock);
    if (!priv->conn)
        open = FALSE;
    g_mutex_unlock(priv->lock);
    return open;
}

void gvir_connection_close(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    DEBUG("Close GVirConnection=%p", conn);

    g_mutex_lock(priv->lock);

    if (priv->domains) {
        g_hash_table_unref(priv->domains);
        priv->domains = NULL;
    }

    if (priv->conn) {
        virConnectClose(priv->conn);
        priv->conn = NULL;
    }
    /* xxx signals */

    g_mutex_unlock(priv->lock);

    g_signal_emit_by_name(conn, "vir-connection-closed");
}

/**
 * gvir_connection_fetch_domains:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 */
gboolean gvir_connection_fetch_domains(GVirConnection *conn,
                                       GCancellable *cancellable,
                                       GError **err)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTable *doms;
    gchar **inactive = NULL;
    gint ninactive = 0;
    gint *active = NULL;
    gint nactive = 0;
    gboolean ret = FALSE;
    gint i;
    virConnectPtr vconn = NULL;

    g_mutex_lock(priv->lock);
    if (!priv->conn) {
        *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                              0,
                              "Connection is not open");
        g_mutex_unlock(priv->lock);
        goto cleanup;
    }
    vconn = priv->conn;
    /* Stop another thread closing the connection just at the minute */
    virConnectRef(vconn);
    g_mutex_unlock(priv->lock);

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        goto cleanup;

    if ((nactive = virConnectNumOfDomains(vconn)) < 0) {
        *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                              0,
                              "Unable to count domains");
        goto cleanup;
    }
    if (nactive) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        active = g_new(gint, nactive);
        if ((nactive = virConnectListDomains(vconn, active, nactive)) < 0) {
            *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                                  0,
                                  "Unable to list domains");
            goto cleanup;
        }
    }

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        goto cleanup;

    if ((ninactive = virConnectNumOfDefinedDomains(vconn)) < 0) {
        *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                              0,
                              "Unable to count domains");
        goto cleanup;
    }

    if (ninactive) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        inactive = g_new(gchar *, ninactive);
        if ((ninactive = virConnectListDefinedDomains(vconn, inactive, ninactive)) < 0) {
            *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                                  0,
                                  "Unable to list domains %d", ninactive);
            goto cleanup;
        }
    }

    doms = g_hash_table_new_full(g_str_hash,
                                 g_str_equal,
                                 g_free,
                                 g_object_unref);

    for (i = 0 ; i < nactive ; i++) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        virDomainPtr vdom = virDomainLookupByID(vconn, active[i]);
        GVirDomain *dom;
        if (!vdom)
            continue;

        dom = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", vdom,
                                       NULL));

        g_hash_table_insert(doms,
                            g_strdup(gvir_domain_get_uuid(dom)),
                            dom);
    }

    for (i = 0 ; i < ninactive ; i++) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        virDomainPtr vdom = virDomainLookupByName(vconn, inactive[i]);
        GVirDomain *dom;
        if (!vdom)
            continue;

        dom = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", vdom,
                                       NULL));

        g_hash_table_insert(doms,
                            g_strdup(gvir_domain_get_uuid(dom)),
                            dom);
    }

    g_mutex_lock(priv->lock);
    if (priv->domains)
        g_hash_table_unref(priv->domains);
    priv->domains = doms;
    virConnectClose(vconn);
    g_mutex_unlock(priv->lock);

    ret = TRUE;

cleanup:
    g_free(active);
    for (i = 0 ; i < ninactive ; i++)
        g_free(inactive[i]);
    g_free(inactive);
    return ret;
}


static void
gvir_connection_fetch_domains_helper(GSimpleAsyncResult *res,
                                     GObject *object,
                                     GCancellable *cancellable)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GError *err = NULL;

    if (!gvir_connection_fetch_domains(conn, cancellable, &err)) {
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}


/**
 * gvir_connection_fetch_domains_async:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (transfer none): completion callback
 * @opaque: (transfer none)(allow-none): opaque data for callback
 */
void gvir_connection_fetch_domains_async(GVirConnection *conn,
                                         GCancellable *cancellable,
                                         GAsyncReadyCallback callback,
                                         gpointer opaque)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(conn),
                                    callback,
                                    opaque,
                                    gvir_connection_fetch_domains);
    g_simple_async_result_run_in_thread(res,
                                        gvir_connection_fetch_domains_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}

/**
 * gvir_connection_fetch_domains_finish:
 * @conn: the connection
 * @result: (transfer none): async method result
 */
gboolean gvir_connection_fetch_domains_finish(GVirConnection *conn,
                                              GAsyncResult *result,
                                              GError **err)
{
    g_return_val_if_fail(GVIR_IS_CONNECTION(conn), FALSE);
    g_return_val_if_fail(G_IS_ASYNC_RESULT(result), FALSE);

    if (G_IS_SIMPLE_ASYNC_RESULT(result)) {
        GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT(result);
        g_warn_if_fail (g_simple_async_result_get_source_tag(simple) == gvir_connection_fetch_domains);
        if (g_simple_async_result_propagate_error(simple, err))
            return FALSE;
    }

    return TRUE;
}


const gchar *gvir_connection_get_uri(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    return priv->uri;
}


static void gvir_domain_ref(gpointer obj, gpointer ignore G_GNUC_UNUSED)
{
    g_object_ref(obj);
}

/**
 * gvir_connection_get_domains:
 *
 * Return value: (element-type LibvirtGObject.Domain) (transfer full): List of #GVirDomain
 */
GList *gvir_connection_get_domains(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    GList *domains;
    g_mutex_lock(priv->lock);
    domains = g_hash_table_get_values(priv->domains);
    g_list_foreach(domains, gvir_domain_ref, NULL);
    g_mutex_unlock(priv->lock);
    return domains;
}

/**
 * gvir_connection_get_domain:
 * @uuid: uuid string of the requested domain
 *
 * Return value: (transfer full): the #GVirDomain, or NULL
 */
GVirDomain *gvir_connection_get_domain(GVirConnection *conn,
                                       const gchar *uuid)
{
    GVirConnectionPrivate *priv = conn->priv;
    GVirDomain *dom;
    g_mutex_lock(priv->lock);
    dom = g_hash_table_lookup(priv->domains, uuid);
    if (dom)
        g_object_ref(dom);
    g_mutex_unlock(priv->lock);
    return dom;
}


/**
 * gvir_connection_find_domain_by_id:
 * @id: id of the requested domain
 *
 * Return value: (transfer full): the #GVirDomain, or NULL
 */
GVirDomain *gvir_connection_find_domain_by_id(GVirConnection *conn,
                                              gint id)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTableIter iter;
    gpointer key, value;

    g_mutex_lock(priv->lock);
    g_hash_table_iter_init(&iter, priv->domains);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GVirDomain *dom = value;
        gint thisid = gvir_domain_get_id(dom, NULL);

        if (thisid == id) {
            g_object_ref(dom);
            g_mutex_unlock(priv->lock);
            return dom;
        }
    }
    g_mutex_unlock(priv->lock);

    return NULL;
}


/**
 * gvir_connection_find_domain_by_name:
 * @name: name of the requested domain
 *
 * Return value: (transfer full): the #GVirDomain, or NULL
 */
GVirDomain *gvir_connection_find_domain_by_name(GVirConnection *conn,
                                                const gchar *name)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTableIter iter;
    gpointer key, value;

    g_mutex_lock(priv->lock);
    g_hash_table_iter_init(&iter, priv->domains);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GVirDomain *dom = value;
        const gchar *thisname = gvir_domain_get_name(dom);

        if (strcmp(thisname, name) == 0) {
            g_object_ref(dom);
            g_mutex_unlock(priv->lock);
            return dom;
        }
    }
    g_mutex_unlock(priv->lock);

    return NULL;
}

static gpointer
gvir_connection_handle_copy(gpointer src)
{
    virConnectRef(src);
    return src;
}


GType gvir_connection_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirConnectionHandle",
             gvir_connection_handle_copy,
             (GBoxedFreeFunc)virConnectClose);

    return handle_type;
}

/**
 * gvir_connection_get_stream:
 * @flags: flags to use for the stream
 *
 * Return value: (transfer full): a #GVirStream stream, or NULL
 */
GVirStream *gvir_connection_get_stream(GVirConnection *self,
                                       gint flags)
{
    GVirConnectionClass *klass;

    g_return_val_if_fail(GVIR_IS_CONNECTION(self), NULL);
    g_return_val_if_fail(self->priv->conn, NULL);

    klass = GVIR_CONNECTION_GET_CLASS(self);
    g_return_val_if_fail(klass->stream_new, NULL);

    virStreamPtr st = virStreamNew(self->priv->conn, flags);

    return klass->stream_new(self, st);
}
