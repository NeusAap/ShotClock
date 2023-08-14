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
#include "stm32f1xx_hal.h"

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
#define BUZZER_Pin GPIO_PIN_15
#define BUZZER_GPIO_Port GPIOC
#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOC
#define BUTTON2_Pin GPIO_PIN_1
#define BUTTON2_GPIO_Port GPIOC
#define BUTTON3_Pin GPIO_PIN_2
#define BUTTON3_GPIO_Port GPIOC
#define BATT_LVL_ORANGE_LED_Pin GPIO_PIN_3
#define BATT_LVL_ORANGE_LED_GPIO_Port GPIOC
#define DISP1_SEG_A_Pin GPIO_PIN_0
#define DISP1_SEG_A_GPIO_Port GPIOA
#define DISP1_SEG_B_Pin GPIO_PIN_1
#define DISP1_SEG_B_GPIO_Port GPIOA
#define DISP1_SEG_C_Pin GPIO_PIN_2
#define DISP1_SEG_C_GPIO_Port GPIOA
#define DISP1_SEG_D_Pin GPIO_PIN_3
#define DISP1_SEG_D_GPIO_Port GPIOA
#define BATT_VOLT_DIV_Pin GPIO_PIN_4
#define BATT_VOLT_DIV_GPIO_Port GPIOA
#define DISP1_SEG_E_Pin GPIO_PIN_5
#define DISP1_SEG_E_GPIO_Port GPIOA
#define DISP1_SEG_F_Pin GPIO_PIN_6
#define DISP1_SEG_F_GPIO_Port GPIOA
#define DISP1_SEG_G_Pin GPIO_PIN_7
#define DISP1_SEG_G_GPIO_Port GPIOA
#define DISP2_SEG_A_Pin GPIO_PIN_0
#define DISP2_SEG_A_GPIO_Port GPIOB
#define DISP2_SEG_B_Pin GPIO_PIN_1
#define DISP2_SEG_B_GPIO_Port GPIOB
#define DISP2_SEG_C_Pin GPIO_PIN_2
#define DISP2_SEG_C_GPIO_Port GPIOB
#define PWM3_BUZZER_Pin GPIO_PIN_6
#define PWM3_BUZZER_GPIO_Port GPIOC
#define DISP1_SEG_DP_Pin GPIO_PIN_8
#define DISP1_SEG_DP_GPIO_Port GPIOA
#define TIM2_CH1_PWM1_SEG_Pin GPIO_PIN_15
#define TIM2_CH1_PWM1_SEG_GPIO_Port GPIOA
#define TIM2_CH2_PWM2_SEG_Pin GPIO_PIN_3
#define TIM2_CH2_PWM2_SEG_GPIO_Port GPIOB
#define DISP2_SEG_D_Pin GPIO_PIN_4
#define DISP2_SEG_D_GPIO_Port GPIOB
#define DISP2_SEG_E_Pin GPIO_PIN_5
#define DISP2_SEG_E_GPIO_Port GPIOB
#define DISP2_SEG_F_Pin GPIO_PIN_6
#define DISP2_SEG_F_GPIO_Port GPIOB
#define DISP2_SEG_G_Pin GPIO_PIN_7
#define DISP2_SEG_G_GPIO_Port GPIOB
#define DISP2_SEG_DP_Pin GPIO_PIN_8
#define DISP2_SEG_DP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
