#include <main.h>

#define KEY_NOT_PRESSED 0x00
#define KEY_PRESSED     0x01

__IO uint8_t
UserButtonPressed = 0x00;
__IO uint32_t
PressCount = 0;
USBD_HandleTypeDef USBD_Device;

static void SystemClock_Config(void);

int main(void)
{
	/* STM32F3xx HAL library initialization
	 * - Configure the Flash prefetch
	 * - Systick timer is configured by default as source of time base, but
	 * user can eventually implement his proper time base source (a general
	 * purpose timer for example or other time source), keeping in mind that
	 * Time base duration should be kept 1ms since PPP_TIMEOUT_VALUEs are
	 * defined and handled in milliseconds basis.
	 * - Set NVIC Group Priority to 4
	 * - Low Level Initialization
	 */
	HAL_Init();
	
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED7);
	BSP_LED_Init(LED9);
	BSP_LED_Init(LED10);
	BSP_LED_Init(LED8);
	BSP_LED_Init(LED6);
	
	SystemClock_Config();

	USBD_Init(&USBD_Device, &HID_Desc, 0);
	USBD_RegisterClass(&USBD_Device, &USBD_HID);
	USBD_Start(&USBD_Device);
	
	HAL_Delay(1000);
	
	for (;;) 
	{
		int16_t accelbuf[3] = { 0 };
		int16_t gyrobuf[3] = { 0 };

		BSP_ACCELERO_GetXYZ(accelbuf);
		BSP_GYRO_GetXYZ(gyrobuf);

		USB_Demo(); 
	}
}

static void cycle_leds()
{
	BSP_LED_Toggle(LED4);
	HAL_Delay(20);
	BSP_LED_Toggle(LED3);
	HAL_Delay(20);
	BSP_LED_Toggle(LED5);
	HAL_Delay(20);
	BSP_LED_Toggle(LED7);
	HAL_Delay(20);
	BSP_LED_Toggle(LED9);
	HAL_Delay(20);
	BSP_LED_Toggle(LED10);
	HAL_Delay(20);
	BSP_LED_Toggle(LED8);
	HAL_Delay(20);
	BSP_LED_Toggle(LED6);
	HAL_Delay(20);
}

static void reset_leds()
{
	BSP_LED_Off(LED3);
	BSP_LED_Off(LED5);
	BSP_LED_Off(LED7);
	BSP_LED_Off(LED9);
	BSP_LED_Off(LED10);
	BSP_LED_Off(LED8);
	BSP_LED_Off(LED6);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
	
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) !=
	    HAL_OK) { Error_Handler(); }
	
	/* Configures the USB clock */
	HAL_RCCEx_GetPeriphCLKConfig(&RCC_PeriphClkInit);
	RCC_PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
	
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and
	 * PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType =
		(RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
		 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
		Error_Handler();
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == USER_BUTTON_PIN) {
		UserButtonPressed++;
		if (UserButtonPressed > 0x2) UserButtonPressed = 0x0;
	}
}

void Error_Handler(void)
{
	while (1) {}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(char* file, uint32_t line)
{
  while (1) {}
}
#endif
