/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gamepad_hid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void StartDefaultTasks();
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
    /* USER CODE BEGIN StartDefaultTask */
void StartDefaultTasks(){

  GamepadReport_t test_report = {0};

  // Trạng thái cân bằng của joystick là 128
  test_report.left_x = 128;
  test_report.left_y = 128;
  test_report.right_x = 128;
  test_report.right_y = 128;

  uint8_t counter = 0;

  for(;;)
  {
    // Gửi báo cáo
    Gamepad_SendReport(&test_report);

    // Tạo giả lập gạt cần trái-phải cho PC test
    test_report.left_x = 128 + (counter % 100);

    // Giả lập nhấn nút tuần tự
    if (counter % 20 == 0) {
       test_report.buttons = (test_report.buttons == 0) ? 0x01 : (test_report.buttons << 1);
    }

    counter++;
    osDelay(10); // Đợi 10ms (100Hz)
  }
}
  /* USER CODE END StartDefaultTask */
/* USER CODE END FunctionPrototypes */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

