/*
 * test-domain-create.c: test libvirt-gconfig domain creation
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * The Software is provided "as is", without warranty of any kind, express
 * or implied, including but not limited to the warranties of
 * merchantability, fitness for a particular purpose and noninfringement.
 * In no event shall the authors or copyright holders be liable for any
 * claim, damages or other liability, whether in an action of contract,
 * tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 *
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include <string.h>
#include <libvirt-gconfig/libvirt-gconfig.h>

const char *features[] = { "foo", "bar", "baz", NULL };

#define g_str_const_check(str1, str2) G_STMT_START { \
    g_assert((str1) != NULL); \
    g_assert(g_strcmp0((str1), (str2)) == 0); \
} G_STMT_END

#define g_str_check(str1, str2) G_STMT_START { \
    char *alloced_str = (str1); \
    g_str_const_check(alloced_str, (str2)); \
    g_free(alloced_str); \
} G_STMT_END


static GVirConfigDomainControllerUsb *
create_usb_controller(GVirConfigDomainControllerUsbModel model, guint indx,
                      GVirConfigDomainControllerUsb *master, guint start_port,
                      guint domain, guint bus, guint slot, guint function,
                      gboolean multifunction)
{
    GVirConfigDomainControllerUsb *controller;
    GVirConfigDomainAddressPci *address;

    controller = gvir_config_domain_controller_usb_new();
    gvir_config_domain_controller_usb_set_model(controller, model);
    gvir_config_domain_controller_set_index(GVIR_CONFIG_DOMAIN_CONTROLLER(controller), indx);
    if (master)
        gvir_config_domain_controller_usb_set_master(controller, master, start_port);
    address = gvir_config_domain_address_pci_new();
    gvir_config_domain_address_pci_set_domain(address, domain);
    gvir_config_domain_address_pci_set_bus(address, bus);
    gvir_config_domain_address_pci_set_slot(address, slot);
    gvir_config_domain_address_pci_set_function(address, function);
    if (multifunction)
        gvir_config_domain_address_pci_set_multifunction(address, multifunction);
    gvir_config_domain_controller_set_address(GVIR_CONFIG_DOMAIN_CONTROLLER(controller),
                                              GVIR_CONFIG_DOMAIN_ADDRESS(address));
    g_object_unref(G_OBJECT(address));

    return controller;
}

static GVirConfigDomainRedirdev *
create_redirdev(guint bus, guint port)
{
    GVirConfigDomainRedirdev *redirdev;
    GVirConfigDomainAddressUsb *address;
    GVirConfigDomainChardevSourceSpiceVmc *spicevmc;
    gchar *port_str;

    redirdev = gvir_config_domain_redirdev_new();
    gvir_config_domain_redirdev_set_bus(redirdev,
                                        GVIR_CONFIG_DOMAIN_REDIRDEV_BUS_USB);
    spicevmc = gvir_config_domain_chardev_source_spicevmc_new();
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(redirdev),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(spicevmc));
    g_object_unref(G_OBJECT(spicevmc));

    address = gvir_config_domain_address_usb_new();
    gvir_config_domain_address_usb_set_bus(address, bus);
    port_str = g_strdup_printf("%d", port);
    gvir_config_domain_address_usb_set_port(address, port_str);
    g_free(port_str);
    gvir_config_domain_redirdev_set_address(redirdev,
                                            GVIR_CONFIG_DOMAIN_ADDRESS(address));
    g_object_unref(G_OBJECT(address));

    return redirdev;
}

int main(int argc, char **argv)
{
    GVirConfigDomain *domain;
    GStrv feat;
    unsigned int i;
    char *xml;

    gvir_config_init(&argc, &argv);

    domain = gvir_config_domain_new();
    g_assert(domain != NULL);
    gvir_config_domain_set_name(domain, "foo");
    g_str_const_check(gvir_config_domain_get_name(domain), "foo");

    gvir_config_domain_set_memory(domain, 1234);
    g_assert(gvir_config_domain_get_memory(domain) == 1234);
    gvir_config_domain_set_vcpus(domain, 3);
    g_assert(gvir_config_domain_get_vcpus(domain) == 3);

    gvir_config_domain_set_features(domain, (const GStrv)features);
    feat = gvir_config_domain_get_features(domain);
    for (i = 0; features[i] != NULL; i++) {
        g_assert(feat[i] != NULL);
        g_assert(strcmp(feat[i], features[i]) == 0);
    }
    g_strfreev(feat);

    gvir_config_domain_set_lifecycle(domain,
                                     GVIR_CONFIG_DOMAIN_LIFECYCLE_ON_POWEROFF,
                                     GVIR_CONFIG_DOMAIN_LIFECYCLE_RESTART);

    /* clock node */
    GVirConfigDomainClock *klock;
    GVirConfigDomainTimerPit *pit;
    GVirConfigDomainTimerRtc *rtc;

    klock = gvir_config_domain_clock_new();
    gvir_config_domain_clock_set_offset(klock, GVIR_CONFIG_DOMAIN_CLOCK_UTC);

    pit = gvir_config_domain_timer_pit_new();
    gvir_config_domain_timer_set_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(pit),
                                             GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);
    gvir_config_domain_clock_add_timer(klock, GVIR_CONFIG_DOMAIN_TIMER(pit));
    g_assert(gvir_config_domain_timer_get_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(pit)) == GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);
    g_object_unref(G_OBJECT(pit));

    rtc = gvir_config_domain_timer_rtc_new();
    gvir_config_domain_timer_set_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(rtc),
                                             GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_CATCHUP);
    gvir_config_domain_clock_add_timer(klock, GVIR_CONFIG_DOMAIN_TIMER(rtc));
    g_assert(gvir_config_domain_timer_get_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(rtc)) == GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_CATCHUP);
    g_object_unref(G_OBJECT(rtc));

    gvir_config_domain_set_clock(domain, klock);
    g_object_unref(G_OBJECT(klock));

    /* os node */
    GVirConfigDomainOs *os;
    GList *devices = NULL;

    os = gvir_config_domain_os_new();
    gvir_config_domain_os_set_os_type(os, GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);
    gvir_config_domain_os_set_arch(os, "x86_64");
    devices = g_list_append(devices,
                             GINT_TO_POINTER(GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_CDROM));
    devices = g_list_append(devices,
                            GINT_TO_POINTER(GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_NETWORK));
    gvir_config_domain_os_set_boot_devices(os, devices);
    g_list_free(devices);
    devices = NULL;
    gvir_config_domain_set_os(domain, os);
    g_object_unref(G_OBJECT(os));

    /* disk node */
    GVirConfigDomainDisk *disk;

    disk = gvir_config_domain_disk_new();
    gvir_config_domain_disk_set_type(disk, GVIR_CONFIG_DOMAIN_DISK_FILE);
    gvir_config_domain_disk_set_guest_device_type(disk, GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);
    gvir_config_domain_disk_set_source(disk, "/tmp/foo/bar");
    gvir_config_domain_disk_set_startup_policy (disk, GVIR_CONFIG_DOMAIN_DISK_STARTUP_POLICY_REQUISITE);
    gvir_config_domain_disk_set_driver_name(disk, "foo");
    gvir_config_domain_disk_set_driver_type(disk, "bar");
    gvir_config_domain_disk_set_driver_name(disk, "qemu");
    gvir_config_domain_disk_set_driver_cache(disk, GVIR_CONFIG_DOMAIN_DISK_CACHE_NONE);
    gvir_config_domain_disk_set_driver_type(disk, "qcow2");
    gvir_config_domain_disk_set_target_bus(disk, GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);
    gvir_config_domain_disk_set_target_dev(disk, "hda");
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(disk));

    g_assert(gvir_config_domain_disk_get_disk_type(disk) == GVIR_CONFIG_DOMAIN_DISK_FILE);
    g_assert(gvir_config_domain_disk_get_guest_device_type(disk) == GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);
    g_assert(gvir_config_domain_disk_get_startup_policy (disk) == GVIR_CONFIG_DOMAIN_DISK_STARTUP_POLICY_REQUISITE);
    g_str_const_check(gvir_config_domain_disk_get_source(disk), "/tmp/foo/bar");
    g_assert(gvir_config_domain_disk_get_driver_cache(disk) == GVIR_CONFIG_DOMAIN_DISK_CACHE_NONE);
    g_str_const_check(gvir_config_domain_disk_get_driver_name(disk), "qemu");
    g_str_const_check(gvir_config_domain_disk_get_driver_type(disk), "qcow2");
    g_assert(gvir_config_domain_disk_get_target_bus(disk) == GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);
    g_str_const_check(gvir_config_domain_disk_get_target_dev(disk), "hda");


    /* network interfaces node */
    GVirConfigDomainInterface *interface;

    interface = GVIR_CONFIG_DOMAIN_INTERFACE(gvir_config_domain_interface_network_new());
    gvir_config_domain_interface_network_set_source(GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK(interface),
                                                    "default");
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(interface));

    interface = GVIR_CONFIG_DOMAIN_INTERFACE(gvir_config_domain_interface_user_new());
    gvir_config_domain_interface_set_ifname(interface, "eth0");
    gvir_config_domain_interface_set_link_state(interface,
                                                GVIR_CONFIG_DOMAIN_INTERFACE_LINK_STATE_UP);
    gvir_config_domain_interface_set_mac(interface, "00:11:22:33:44:55");
    gvir_config_domain_interface_set_model(interface, "foo");
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(interface));


    /* input node */
    GVirConfigDomainInput *input;

    input = gvir_config_domain_input_new();
    gvir_config_domain_input_set_device_type(input,
                                             GVIR_CONFIG_DOMAIN_INPUT_DEVICE_TABLET);
    gvir_config_domain_input_set_bus(input, GVIR_CONFIG_DOMAIN_INPUT_BUS_USB);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(input));

    /* graphics node */
    GVirConfigDomainGraphicsSpice *graphics;

    graphics = gvir_config_domain_graphics_spice_new();
    gvir_config_domain_graphics_spice_set_port(graphics, 1234);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(graphics));

    /* video node */
    GVirConfigDomainVideo *video;

    video = gvir_config_domain_video_new();
    gvir_config_domain_video_set_model(video,
                                       GVIR_CONFIG_DOMAIN_VIDEO_MODEL_QXL);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(video));

    /* sound node */
    GVirConfigDomainSound *sound;

    sound = gvir_config_domain_sound_new();
    gvir_config_domain_sound_set_model(sound,
                                       GVIR_CONFIG_DOMAIN_SOUND_MODEL_ES1370);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(sound));

    /* console node */
    GVirConfigDomainConsole *console;
    GVirConfigDomainChardevSourcePty *pty;

    console = gvir_config_domain_console_new();
    pty = gvir_config_domain_chardev_source_pty_new();
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(console),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(pty));
    g_object_unref(G_OBJECT(pty));
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(console));

    /* spice agent channel */
    GVirConfigDomainChannel *channel;
    GVirConfigDomainChardevSourceSpiceVmc *spicevmc;

    channel = gvir_config_domain_channel_new();
    gvir_config_domain_channel_set_target_type(channel,
                                               GVIR_CONFIG_DOMAIN_CHANNEL_TARGET_VIRTIO);
    spicevmc = gvir_config_domain_chardev_source_spicevmc_new();
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(channel),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(spicevmc));
    g_object_unref(G_OBJECT(spicevmc));
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(channel));

    /* spice usb redirection */
    GVirConfigDomainControllerUsb *ehci;
    GVirConfigDomainControllerUsb *uhci1;
    GVirConfigDomainControllerUsb *uhci2;
    GVirConfigDomainControllerUsb *uhci3;
    GVirConfigDomainRedirdev *redirdev;

    ehci = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_EHCI1,
                                 1, NULL, 0, 0, 0, 8, 7, FALSE);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(ehci));
    uhci1 = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI1,
                                  7, ehci, 0, 0, 0, 8, 0, TRUE);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(uhci1));
    uhci2 = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI2,
                                  7, ehci, 2, 0, 0, 8, 1, FALSE);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(uhci2));
    uhci3 = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI3,
                                  7, ehci, 4, 0, 0, 8, 2, FALSE);
    g_assert(gvir_config_domain_controller_get_index(GVIR_CONFIG_DOMAIN_CONTROLLER(uhci1)) == 1);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(uhci3));


    /* three redirdev channels allows to redirect a maximum of 3 USB
     * devices at a time. The address which create_redirdev assigns to the
     * redirdev object is optional
     */
    redirdev = create_redirdev(0, 3);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(redirdev));
    redirdev = create_redirdev(0, 4);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(redirdev));
    redirdev = create_redirdev(0, 5);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(redirdev));

    gvir_config_domain_set_devices(domain, devices);
    g_list_foreach(devices, (GFunc)g_object_unref, NULL);
    g_list_free(devices);
    devices = NULL;

    gvir_config_domain_set_custom_xml(domain, "<foo/>", "ns", "http://foo", NULL);
    gvir_config_domain_set_custom_xml(domain, "<foo/>", "nsbar", "http://bar", NULL);
    gvir_config_domain_set_custom_xml(domain, "<foo/>", "ns", "http://bar", NULL);
    gvir_config_domain_set_custom_xml(domain, "<bar/>", "ns", "http://foo", NULL);

    g_str_check(gvir_config_domain_get_custom_xml(domain, "http://foo"), "<ns:bar xmlns:ns=\"http://foo\"/>");
    g_str_check(gvir_config_domain_get_custom_xml(domain, "http://bar"), "<ns:foo xmlns:ns=\"http://bar\"/>");

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(domain));
    g_print("%s\n\n", xml);
    g_free(xml);
    g_object_unref(G_OBJECT(domain));


    /* storage pool */
    GVirConfigStoragePool *pool;
    GVirConfigStoragePoolSource *pool_source;
    GVirConfigStoragePoolTarget *pool_target;
    GVirConfigStoragePermissions *perms;

    pool = gvir_config_storage_pool_new();
    gvir_config_storage_pool_set_pool_type(pool, GVIR_CONFIG_STORAGE_POOL_TYPE_DIR);

    pool_source = gvir_config_storage_pool_source_new();
    gvir_config_storage_pool_source_set_directory(pool_source, "/foo/bar");
    gvir_config_storage_pool_set_source(pool, pool_source);
    g_object_unref(G_OBJECT(pool_source));

    perms = gvir_config_storage_permissions_new();
    gvir_config_storage_permissions_set_owner(perms, 1001);
    gvir_config_storage_permissions_set_group(perms, 1007);
    gvir_config_storage_permissions_set_mode(perms, 0744);
    gvir_config_storage_permissions_set_label(perms, "virt_image_t");

    pool_target = gvir_config_storage_pool_target_new();
    gvir_config_storage_pool_target_set_path(pool_target, "/dev/disk/by-path");
    gvir_config_storage_pool_target_set_permissions(pool_target, perms);
    g_object_unref(G_OBJECT(perms));
    gvir_config_storage_pool_set_target(pool, pool_target);
    g_object_unref(G_OBJECT(pool_target));

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(pool));
    g_print("%s\n\n", xml);
    g_free(xml);
    g_object_unref(G_OBJECT(pool));


    /* storage volume */
    GVirConfigStorageVol *vol;
    GVirConfigStorageVolTarget *vol_target;

    vol = gvir_config_storage_vol_new();
    gvir_config_storage_vol_set_name(vol, "my-volume");
    gvir_config_storage_vol_set_capacity(vol, 1000000);

    perms = gvir_config_storage_permissions_new();
    gvir_config_storage_permissions_set_owner(perms, 1001);
    gvir_config_storage_permissions_set_group(perms, 1007);
    gvir_config_storage_permissions_set_mode(perms, 0744);
    gvir_config_storage_permissions_set_label(perms, "virt_image_t");

    vol_target = gvir_config_storage_vol_target_new();
    gvir_config_storage_vol_target_set_format(vol_target, "qcow2");
    gvir_config_storage_vol_target_set_permissions(vol_target, perms);
    g_object_unref(G_OBJECT(perms));
    gvir_config_storage_vol_set_target(vol, vol_target);
    g_object_unref(G_OBJECT(vol_target));

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(vol));
    g_print("%s\n\n", xml);
    g_free(xml);
    g_object_unref(G_OBJECT(vol));

    return 0;
}
