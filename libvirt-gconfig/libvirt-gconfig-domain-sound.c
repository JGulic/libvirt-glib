/*
 * libvirt-gconfig-domain-sound.c: libvirt domain sound configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_SOUND_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_SOUND, GVirConfigDomainSoundPrivate))

struct _GVirConfigDomainSoundPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainSound, gvir_config_domain_sound, GVIR_TYPE_CONFIG_DOMAIN_DEVICE);


static void gvir_config_domain_sound_class_init(GVirConfigDomainSoundClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainSoundPrivate));
}


static void gvir_config_domain_sound_init(GVirConfigDomainSound *sound)
{
    g_debug("Init GVirConfigDomainSound=%p", sound);

    sound->priv = GVIR_CONFIG_DOMAIN_SOUND_GET_PRIVATE(sound);
}


GVirConfigDomainSound *gvir_config_domain_sound_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_SOUND,
                                    "sound", NULL);
    return GVIR_CONFIG_DOMAIN_SOUND(object);
}

GVirConfigDomainSound *gvir_config_domain_sound_new_from_xml(const gchar *xml,
                                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_SOUND,
                                             "sound", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_SOUND(object);
}

void gvir_config_domain_sound_set_model(GVirConfigDomainSound *sound,
                                        GVirConfigDomainSoundModel model)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_SOUND(sound));
    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(sound),
                                            "sound");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "model",
                                               GVIR_TYPE_CONFIG_DOMAIN_SOUND_MODEL,
                                               model,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}
