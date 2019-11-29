#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <stm32f303xc.h>
#include <stm32f3_discovery.h>
#include <stm32f3_discovery_gyroscope.h>
#include <stm32f3_discovery_accelerometer.h>
#include <mems.h>
#include <selftest.h>
#include <usbd_core.h>
#include <usbd_desc.h>
#include <usbd_hid.h>

typedef struct
{
	void (*DemoFunc)(void);
	uint8_t DemoName[50];
	uint32_t DemoIndex;
} BSP_DemoTypedef;

#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

#define USE_USB_INTERRUPT_DEFAULT

void Toggle_Leds(void);

void Error_Handler(void);

#endif /* __MAIN_H */

