/*
 * libvirt-gconfig-domain-graphics-sdl.c: libvirt domain SDL configuration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_SDL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SDL, GVirConfigDomainGraphicsSdlPrivate))

struct _GVirConfigDomainGraphicsSdlPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainGraphicsSdl, gvir_config_domain_graphics_sdl, GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_sdl_class_init(GVirConfigDomainGraphicsSdlClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainGraphicsSdlPrivate));
}


static void gvir_config_domain_graphics_sdl_init(GVirConfigDomainGraphicsSdl *graphics_sdl)
{
    g_debug("Init GVirConfigDomainGraphicsSdl=%p", graphics_sdl);

    graphics_sdl->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_SDL_GET_PRIVATE(graphics_sdl);
}


GVirConfigDomainGraphicsSdl *gvir_config_domain_graphics_sdl_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SDL,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "sdl", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SDL(object);
}

GVirConfigDomainGraphicsSdl *
gvir_config_domain_graphics_sdl_new_from_xml(const gchar *xml,
                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SDL,
                                             "graphics", NULL, xml, error);
    if (object == NULL)
        return NULL;
    gvir_config_object_set_attribute(object, "type", "sdl", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SDL(object);
}

void gvir_config_domain_graphics_sdl_set_xauthority(GVirConfigDomainGraphicsSdl *graphics,
                                                    const gchar *path)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_GRAPHICS_SDL(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "xauth", path,
                                     NULL);
}

void gvir_config_domain_graphics_sdl_set_display(GVirConfigDomainGraphicsSdl *graphics,
                                                 const gchar *disp)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_GRAPHICS_SDL(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "display", disp,
                                     NULL);
}
