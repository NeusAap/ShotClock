/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BOOT0_IN_Pin GPIO_PIN_13
#define BOOT0_IN_GPIO_Port GPIOC
#define ADC_BATT_IN_Pin GPIO_PIN_0
#define ADC_BATT_IN_GPIO_Port GPIOA
#define BOOT0_INB0_Pin GPIO_PIN_0
#define BOOT0_INB0_GPIO_Port GPIOB
#define BUTTON1_INT_Pin GPIO_PIN_10
#define BUTTON1_INT_GPIO_Port GPIOB
#define BUTTON1_INT_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON2_INT_Pin GPIO_PIN_11
#define BUTTON2_INT_GPIO_Port GPIOB
#define BUTTON2_INT_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON3_INT_Pin GPIO_PIN_12
#define BUTTON3_INT_GPIO_Port GPIOB
#define BUTTON3_INT_EXTI_IRQn EXTI4_15_IRQn
#define ORANGE_LED_Pin GPIO_PIN_13
#define ORANGE_LED_GPIO_Port GPIOB
#define TLC59116_RESETn_Pin GPIO_PIN_7
#define TLC59116_RESETn_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
