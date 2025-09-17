/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

 #include"ssd1306.h"
#include "../../Drivers/VL53L1_Nucleo/core/inc/vl53l1_api.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

VL53L1_Dev_t devLeft;
VL53L1_Dev_t devRight;
VL53L1_DEV Dev;
VL53L1_DEV Dev2;
uint8_t ToFSensor = 0;
uint8_t ToFSensor2 = 0;
VL53L1_RangingMeasurementData_t RangingData;
VL53L1_RangingMeasurementData_t RangingData2;
uint16_t Dist_Obst_G;
uint16_t Dist_Obst_G2;
uint8_t txData[4];
char str[10];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

void ResetAndInitializeTOF1(void );

void ResetAndInitializeTOF2(void );
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t message;
CAN_TxHeaderTypeDef TxHeader;  // En-tête de la trame reçue
uint8_t TxData[2]; // Octets envoyés
uint16_t value1;
uint16_t value2;
uint32_t TxMailbox; // Identifiant de la mailbox d'émission
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM7_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_CAN1_Init();
  MX_TIM7_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim7);

  	// Démarrage du bus CAN
  	HAL_CAN_Start(&hcan1);

  	// On choisit les callbacks d'interruptions
  	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING );
  	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_TX_MAILBOX_EMPTY );

  	// Configuration des filtres
  	CAN_FilterTypeDef sFilterConfig;
  	sFilterConfig.FilterBank = 0;
  	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST; // On donne une liste d'identifiants
  	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  	// Valeurs de l'ID à filtrer, codée sur 11 bits, alignée à gauche sur 16 bits
  	// ID = 0x11 = 00000010001 (11 bits)
  	// FILTRE = 0x220 = 00000010 00100000 (11 bits alignés à gauche dans un uint16_t)
  	sFilterConfig.FilterIdHigh = 0x0220; // soit 0x11 << 5
  	sFilterConfig.FilterIdLow = 0x0220;
  	sFilterConfig.FilterMaskIdHigh = 0x0220;
  	sFilterConfig.FilterMaskIdLow = 0x0220;
  	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  	sFilterConfig.FilterActivation = ENABLE;
  	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig );

  	// Message à envoyer
  	TxHeader.IDE = CAN_ID_STD; // Identifiant codé sur 11 bits
  	TxHeader.RTR = CAN_RTR_DATA; // Trame de données
  	TxHeader.StdId = 0x11; // Identifiant

  	TxHeader.DLC = 4;
  	txData[0] = (value1 >> 8) & 0xFF;  // high byte of tof1
  	txData[1] = value1 & 0xFF;         // low byte of tof1
  	txData[2] = (value2 >> 8) & 0xFF;  // high byte of tof2
  	txData[3] = value2 & 0xFF;         // low byte of tof2

	 ResetAndInitializeTOF1();

	 ResetAndInitializeTOF2();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 80;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00F12981;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 40000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 2000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, SHUT1_Pin|SHUT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : B1_Pin INTOF2_Pin INTOF1_Pin */
  GPIO_InitStruct.Pin = B1_Pin|INTOF2_Pin|INTOF1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SHUT1_Pin SHUT2_Pin */
  GPIO_InitStruct.Pin = SHUT1_Pin|SHUT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == INTOF1_Pin)
	    {
	        VL53L1_RangingMeasurementData_t RangingData;
	        Dev = &devLeft; // Use the correct device handle
	        VL53L1_GetRangingMeasurementData(Dev, &RangingData);
	        value1 = RangingData.RangeMilliMeter;
	      	txData[0] = (value1 >> 8) & 0xFF;
	      	txData[1] = value1 & 0xFF;



	        // Clear interrupt and start next measurement
	        VL53L1_ClearInterruptAndStartMeasurement(Dev);
	    }
	    else if (GPIO_Pin == INTOF2_Pin)
	    {

	        VL53L1_RangingMeasurementData_t RangingData;
	        Dev = &devRight;
	        VL53L1_GetRangingMeasurementData(Dev, &RangingData);
	        value2 = RangingData.RangeMilliMeter;
	      	txData[2] = (value2 >> 8) & 0xFF;
	      	txData[3] = value2 & 0xFF;

	        VL53L1_ClearInterruptAndStartMeasurement(Dev);
	    }
}

// Callback de fin de transmission d'un message depuis la mailbox 0
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);
}


// Interruption périodique du timer 7, envoi d'un message toutes les secondes
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim7)
	{
		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, txData, &TxMailbox);

	}
}

///


void ResetAndInitializeTOF1(void) {
    VL53L1_DetectionConfig_t DetectionConfig;
    uint8_t newI2C = 0x54; // New I2C address for first sensor

    // Reset both sensors to avoid I2C conflict
    HAL_GPIO_WritePin(GPIOH, SHUT1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOH, SHUT2_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);

    // Bring up only sensor 1
    HAL_GPIO_WritePin(GPIOH, SHUT1_Pin, GPIO_PIN_SET);
    HAL_Delay(5);

    // Assign device handle
    Dev = &devLeft;
    Dev->comms_speed_khz = 400;
    Dev->I2cHandle = &hi2c1;
    Dev->comms_type = 1;
    Dev->I2cDevAddr = 0x52; // Default address

    // Change address to avoid conflict
    VL53L1_SetDeviceAddress(Dev, newI2C);
    Dev->I2cDevAddr = newI2C;

    // Initialize and configure sensor
    VL53L1_WaitDeviceBooted(Dev);
    VL53L1_DataInit(Dev);
    VL53L1_StaticInit(Dev);
    VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
    VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, 40000);
    VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, 50);

    memset(&DetectionConfig, 0, sizeof(DetectionConfig));
    DetectionConfig.DetectionMode = VL53L1_DETECTION_DISTANCE_ONLY;
    DetectionConfig.IntrNoTarget = 0;
    DetectionConfig.Distance.CrossMode = VL53L1_THRESHOLD_IN_WINDOW;
    DetectionConfig.Distance.Low = 0;
    DetectionConfig.Distance.High = 300;
    VL53L1_SetThresholdConfig(Dev, &DetectionConfig);

    VL53L1_UserRoi_t roiConfig;
    roiConfig.TopLeftX = 0;
    roiConfig.TopLeftY = 15;
    roiConfig.BotRightX = 15;
    roiConfig.BotRightY = 0;
    VL53L1_SetUserROI(Dev, &roiConfig);

    VL53L1_ClearInterruptAndStartMeasurement(Dev);
}

void ResetAndInitializeTOF2(void) {
    VL53L1_DetectionConfig_t DetectionConfig;

    // Keep sensor 1 on, but reset sensor 2
    HAL_GPIO_WritePin(GPIOH, SHUT2_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOH, SHUT2_Pin, GPIO_PIN_SET);
    HAL_Delay(5);

    // Assign device handle
    Dev = &devRight;
    Dev->comms_speed_khz = 400;
    Dev->I2cHandle = &hi2c1;
    Dev->comms_type = 1;
    Dev->I2cDevAddr = 0x52; // Keep default address

    // Initialize and configure sensor
    VL53L1_WaitDeviceBooted(Dev);
    VL53L1_DataInit(Dev);
    VL53L1_StaticInit(Dev);
    VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
    VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, 40000);
    VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, 50);

    memset(&DetectionConfig, 0, sizeof(DetectionConfig));
    DetectionConfig.DetectionMode = VL53L1_DETECTION_DISTANCE_ONLY;
    DetectionConfig.IntrNoTarget = 0;
    DetectionConfig.Distance.CrossMode = VL53L1_THRESHOLD_IN_WINDOW;
    DetectionConfig.Distance.Low = 0;
    DetectionConfig.Distance.High = 300;
    VL53L1_SetThresholdConfig(Dev, &DetectionConfig);

    VL53L1_UserRoi_t roiConfig;
    roiConfig.TopLeftX = 0;
    roiConfig.TopLeftY = 15;
    roiConfig.BotRightX = 15;
    roiConfig.BotRightY = 0;
    VL53L1_SetUserROI(Dev, &roiConfig);

    VL53L1_ClearInterruptAndStartMeasurement(Dev);
}





/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
