#ifndef IMU_PERIPHERAL_H
#define IMU_PERIPHERAL_H

#include <iostream>
#include <functional>
#include <optional>
#include <hidapi.h>

struct USBServiceError : public std::exception 
{
    USBServiceError(std::string desc) : desc(desc) {};

    std::string desc;

    virtual const char* what()
    {
        return desc.c_str();
    }
};

class USBService
{
public:
    USBService() 
    {
        
    };

    ~USBService()
    {
        hid_exit();
    };

    void connect(
        std::function<void(std::optional<USBServiceError>)> callback
    ){
        std::optional<USBServiceError> opt_err = std::nullopt;
        wchar_t wstr[256];
        unsigned char in[64];
        int err;

        err = hid_init();
        if (err < 0) 
        {
            opt_err = USBServiceError { 
                "Couldn't init HIDAPI with error code" + std::to_string(err) 
            };
            goto cleanup_exit;
        }

        handle = hid_open(vid, pid, nullptr);
        if (handle == nullptr)
        {
            opt_err = USBServiceError { "Couldn't open the HID device." };
            goto cleanup_exit;
        }

        err = hid_get_manufacturer_string(handle, wstr, MAX_STR);
        std::cout << "Manufacturer: " << wstr << std::endl;

        err = hid_get_product_string(handle, wstr, MAX_STR);
        std::cout << "Product: " << wstr << std::endl;

        err = hid_get_serial_number_string(handle, wstr, MAX_STR);
        std::cout << "SN: " << wstr << std::endl;

        err = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
        std::cout << "Indexed string 1: " << wstr << std::endl;

        err = hid_read(handle, in, 64);

        std::cout << "HID read buffer: " << in << std::endl;

        err = hid_exit();

        return;

    cleanup_exit:
        hid_exit();
    cleanup_callback:
        callback(opt_err);
    }

    void disconnect()
    {
        hid_exit();
    }

private:
    static const int vid = 0x0483;
    static const int pid = 0x5710;

    hid_device* handle;
};

#endif