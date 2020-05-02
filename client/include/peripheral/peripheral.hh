#ifndef IMU_PERIPHERAL_H
#define IMU_PERIPHERAL_H

#include <iostream>
#include <functional>
#include <optional>
#include <libusb.h>

struct PeripheralDeviceError : public std::exception 
{
    PeripheralDeviceError(std::string desc) : desc(desc) {};

    std::string desc;

    virtual const char* what()
    {
        return desc.c_str();
    }
};

class PeripheralDevice
{
public:
    PeripheralDevice()
    {
        int err;

        err = libusb_init(&ctx);
        if (err < 0) 
        {
            throw PeripheralDeviceError { 
                "Couldn't init USB with error code" + std::to_string(err) 
            };
        }

        libusb_set_option(
            ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
    };

    ~PeripheralDevice()
    {
        libusb_exit(ctx);
    };

    void connect(
        std::function<void(std::optional<PeripheralDeviceError>)> callback
    ){
        std::optional<PeripheralDeviceError> opt_err;
        libusb_device_handle *dev_handle;
        int err;

        dev_handle = libusb_open_device_with_vid_pid(ctx, vid, pid);

        if (!dev_handle)
        {
            opt_err = { 
                PeripheralDeviceError { "Peripheral device is not attached. " } 
            };
            goto cleanup;
        }

        if (libusb_kernel_driver_active(dev_handle, 0))
        {
            std::cout << "kernel driver active, trying detach" << std::endl;
            err = libusb_detach_kernel_driver(dev_handle, 0);
            if (err)
            {
                opt_err = { PeripheralDeviceError {
                    "Couldn't detach kernel driver with error code " 
                    + std::to_string(err)
                } };
                goto cleanup;
            }

            std::cout << "detach successful" << std::endl;
        }

        std::cout << "kernel driver inactive" << std::endl;

cleanup:
        libusb_close(dev_handle);
        callback(opt_err);
    }

private:
    static const int vid = 0x0483;
    static const int pid = 0x5710;

    libusb_device *periph;
    libusb_context *ctx;
};

#endif