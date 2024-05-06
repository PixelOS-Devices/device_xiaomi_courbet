/*

   Copyright (C) 2024 The LineageOS Project.
   
 */

#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;

std::vector<std::string> ro_props_default_source_order = {
    "",
    "odm.",
    "product.",
    "system.",
    "system_ext.",
    "vendor.",
    "vendor_dlkm.",
};

void property_override(char const prop[], char const value[], bool add = true)
{
    prop_info *pi;

    pi = (prop_info *) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void set_ro_build_prop(const std::string &prop, const std::string &value) {
    for (const auto &source : ro_props_default_source_order) {
        auto prop_name = "ro." + source + "build." + prop;
        if (source == "")
            property_override(prop_name.c_str(), value.c_str());
        else
            property_override(prop_name.c_str(), value.c_str(), false);
    }
};

void set_ro_product_prop(const std::string &prop, const std::string &value) {
    for (const auto &source : ro_props_default_source_order) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    }
};

void vendor_load_properties() {
    std::string region;
    std::string sku;
    region = GetProperty("ro.boot.hwc", "GLOBAL");
    sku = GetProperty("ro.boot.product.hardware.sku","courbetin");

    std::string model;
    std::string device;
    std::string fingerprint;
    std::string description;
    std::string mod_device;

    if (region == "GLOBAL") {
        model = "Xiaomi 11 Lite";
        device =  "courbet";
        fingerprint = "Xiaomi/courbet/courbet:13/TKQ1.221013.002/V14.0.4.0.TKQMIXM:user/release-keys";
        description = "courbet-user 13 TKQ1.221013.002 V14.0.4.0.TKQMIXM release-keys";
        mod_device = "courbet_global";
        property_override("ro.boot.product.hardware.sku", "courbet");
    } else if (region == "INDIA") {
        model = "Xiaomi 11 Lite";
        device =  "courbetin"; 
        fingerprint = "Xiaomi/courbetin/courbetin:13/TKQ1.221013.002/V14.0.2.0.TKQINXM:user/release-keys";
        description = "courbetin-user 13 TKQ1.221013.002 V14.0.2.0.TKQINXM release-keys";
        mod_device = "courbet_in_global";
    }

    set_ro_build_prop("fingerprint", fingerprint);
    set_ro_product_prop("device", device);
    set_ro_product_prop("model", model);
    property_override("ro.build.description", description.c_str());
    if (mod_device != "") {
        property_override("ro.product.mod_device", mod_device.c_str());
    }
}
