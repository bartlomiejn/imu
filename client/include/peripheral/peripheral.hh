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
    PeripheralDevice() {};

    ~PeripheralDevice()
    {
        libusb_exit(ctx);
    };

    void connect(
        std::function<void(std::optional<PeripheralDeviceError>)> callback
    ){
        std::optional<PeripheralDeviceError> opt_err = std::nullopt;
        int err;

        err = libusb_init(&ctx);
        if (err < 0) 
        {
            opt_err = PeripheralDeviceError { 
                "Couldn't init USB with error code" + std::to_string(err) 
            };
            goto cleanup_callback;
        }

        libusb_set_option(
            ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

        dev_handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
        if (!dev_handle)
        {
            opt_err = { 
                PeripheralDeviceError { "Peripheral device is not attached. " } 
            };
            goto cleanup_exit;
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
                goto cleanup_full;
            }

            std::cout << "detach successful" << std::endl;
        }

        std::cout << "kernel driver inactive" << std::endl;

        err = libusb_claim_interface(dev_handle, 0);
        if (err)
        {
            opt_err = { PeripheralDeviceError {
                "Couldn't claim interface with error code " 
                + std::to_string(err)
            } };
            goto cleanup_full;
        }

        return;

    cleanup_full:
        libusb_close(dev_handle);
    cleanup_exit:
        libusb_exit(ctx);
    cleanup_callback:
        callback(opt_err);
    }

    void disconnect()
    {
        libusb_release_interface(dev_handle, 0);
        libusb_close(dev_handle);
        libusb_exit(ctx);
    }

private:
    static const int vid = 0x0483;
    static const int pid = 0x5710;

    libusb_device_handle *dev_handle;
    libusb_context *ctx;
};

#endif