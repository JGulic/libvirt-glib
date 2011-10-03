/*
 * libvirt-gobject-input-stream.h: libvirt gobject integration
 *
 * Copyright (C) 2011 Red Hat
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-input-stream.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define gvir_input_stream_get_type _gvir_input_stream_get_type
G_DEFINE_TYPE(GVirInputStream, gvir_input_stream, G_TYPE_INPUT_STREAM);

enum
{
    PROP_0,
    PROP_STREAM
};

struct _GVirInputStreamPrivate
{
    GVirStream *stream;

    /* pending operation metadata */
    GSimpleAsyncResult *result;
    GCancellable *cancellable;
    gpointer buffer;
    gsize count;
};

static void gvir_input_stream_get_property(GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
    GVirInputStream *stream = GVIR_INPUT_STREAM(object);

    switch (prop_id) {
    case PROP_STREAM:
        g_value_set_object(value, stream->priv->stream);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_input_stream_set_property(GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
    GVirInputStream *stream = GVIR_INPUT_STREAM(object);

    switch (prop_id) {
    case PROP_STREAM:
        stream->priv->stream = g_value_dup_object(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_input_stream_finalize(GObject *object)
{
    GVirInputStream *stream = GVIR_INPUT_STREAM(object);

    if (stream->priv->stream)
        g_object_unref(stream->priv->stream);

    if (G_OBJECT_CLASS(gvir_input_stream_parent_class)->finalize)
        (*G_OBJECT_CLASS(gvir_input_stream_parent_class)->finalize)(object);
}

static void
gvir_input_stream_read_ready (G_GNUC_UNUSED virStreamPtr st,
                              int events, void *opaque)
{
    GVirInputStream *stream = GVIR_INPUT_STREAM(opaque);
    GVirInputStreamPrivate *priv = stream->priv;
    GSimpleAsyncResult *simple;
    GError *error = NULL;
    gssize result;

    g_return_if_fail(events & VIR_STREAM_EVENT_READABLE);

    result  = gvir_stream_receive(priv->stream, priv->buffer, priv->count,
                                  priv->cancellable, &error);

    if (g_error_matches(error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK)) {
        g_warn_if_reached();
        return;
    }

    simple = stream->priv->result;
    stream->priv->result = NULL;

    if (result >= 0)
        g_simple_async_result_set_op_res_gssize(simple, result);

    if (error)
        g_simple_async_result_take_error(simple, error);

    if (priv->cancellable) {
        g_object_unref(stream->priv->cancellable);
        priv->cancellable = NULL;
    }

    g_simple_async_result_complete(simple);
    g_object_unref(simple);

    return;
}

static void gvir_input_stream_read_async(GInputStream        *stream,
                                         void                *buffer,
                                         gsize                count,
                                         G_GNUC_UNUSED int    io_priority,
                                         GCancellable        *cancellable,
                                         GAsyncReadyCallback  callback,
                                         gpointer             user_data)
{
    GVirInputStream *input_stream = GVIR_INPUT_STREAM(stream);
    virStreamPtr handle;

    g_return_if_fail(GVIR_IS_INPUT_STREAM(stream));
    g_return_if_fail(input_stream->priv->result == NULL);

    g_object_get(input_stream->priv->stream, "handle", &handle, NULL);

    if (virStreamEventAddCallback(handle, VIR_STREAM_EVENT_READABLE,
                                  gvir_input_stream_read_ready, stream, NULL) < 0) {
        g_simple_async_report_error_in_idle(G_OBJECT(stream),
                                            callback,
                                            user_data,
                                            G_IO_ERROR, G_IO_ERROR_INVALID_ARGUMENT,
                                            "Couldn't add event callback %s",
                                            G_STRFUNC);
        goto end;
    }

    input_stream->priv->result =
        g_simple_async_result_new(G_OBJECT(stream), callback, user_data,
                                  gvir_input_stream_read_async);
    if (cancellable)
        g_object_ref(cancellable);
    input_stream->priv->cancellable = cancellable;
    input_stream->priv->buffer = buffer;
    input_stream->priv->count = count;

end:
    virStreamFree(handle);
}


static gssize gvir_input_stream_read_finish(GInputStream  *stream,
                                            GAsyncResult  *result,
                                            G_GNUC_UNUSED GError **error)
{
    GVirInputStream *input_stream = GVIR_INPUT_STREAM(stream);
    GSimpleAsyncResult *simple;
    virStreamPtr handle;
    gssize count;

    g_return_val_if_fail(GVIR_IS_INPUT_STREAM(stream), -1);
    g_object_get(input_stream->priv->stream, "handle", &handle, NULL);

    simple = G_SIMPLE_ASYNC_RESULT(result);

    g_warn_if_fail(g_simple_async_result_get_source_tag(simple) == gvir_input_stream_read_async);

    count = g_simple_async_result_get_op_res_gssize(simple);

    virStreamEventRemoveCallback(handle);
    virStreamFree(handle);

    return count;
}


static void gvir_input_stream_class_init(GVirInputStreamClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GInputStreamClass *ginputstream_class = G_INPUT_STREAM_CLASS(klass);

    g_type_class_add_private(klass, sizeof(GVirInputStreamPrivate));

    gobject_class->finalize = gvir_input_stream_finalize;
    gobject_class->get_property = gvir_input_stream_get_property;
    gobject_class->set_property = gvir_input_stream_set_property;

    ginputstream_class->read_fn = NULL;
    ginputstream_class->read_async = gvir_input_stream_read_async;
    ginputstream_class->read_finish = gvir_input_stream_read_finish;

    g_object_class_install_property(gobject_class, PROP_STREAM,
                                    g_param_spec_object("stream",
                                                        "stream",
                                                        "GVirStream",
                                                        GVIR_TYPE_STREAM, G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

static void gvir_input_stream_init(GVirInputStream *stream)
{
    stream->priv = G_TYPE_INSTANCE_GET_PRIVATE(stream, GVIR_TYPE_INPUT_STREAM, GVirInputStreamPrivate);
}

GVirInputStream* _gvir_input_stream_new(GVirStream *stream)
{
    return GVIR_INPUT_STREAM(g_object_new(GVIR_TYPE_INPUT_STREAM, "stream", stream, NULL));
}
