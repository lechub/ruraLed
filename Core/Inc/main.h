/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f3xx_hal.h"

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
#define rura1_Pin GPIO_PIN_7
#define rura1_GPIO_Port GPIOA
#define rura1_2_Pin GPIO_PIN_10
#define rura1_2_GPIO_Port GPIOB
#define sound_Pin GPIO_PIN_11
#define sound_GPIO_Port GPIOB
#define rura2On_Pin GPIO_PIN_6
#define rura2On_GPIO_Port GPIOC
#define rura3_Pin GPIO_PIN_7
#define rura3_GPIO_Port GPIOC
#define rura1On_Pin GPIO_PIN_8
#define rura1On_GPIO_Port GPIOC
#define in1gnd_Pin GPIO_PIN_9
#define in1gnd_GPIO_Port GPIOC
#define sound_2_Pin GPIO_PIN_3
#define sound_2_GPIO_Port GPIOB
#define rura2_2_Pin GPIO_PIN_4
#define rura2_2_GPIO_Port GPIOB
#define rura2_Pin GPIO_PIN_6
#define rura2_GPIO_Port GPIOB
#define in2gnd_Pin GPIO_PIN_8
#define in2gnd_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
