
#include "../../Drivers/VL53L1_Nucleo/core/inc/vl53l1_api.h"

VL53L1_Dev_t devLeft;
VL53L1_Dev_t devRight;
VL53L1_DEV Dev;
uint8_t ToFSensor = 0;
VL53L1_RangingMeasurementData_t RangingData;
uint16_t Dist_Obst_G;



void ResetAndInitializeTOF1(void ){
VL53L1_DetectionConfig_t DetectionConfig;
uint8_t newI2C = 0x52;
/* Reset the 1 ToF sensors on the expansion board */
HAL_GPIO_WritePin(GPIOH, SHUT1_Pin, GPIO_PIN_RESET);
HAL_Delay(10);
/* Bring the sensors out of the reset stage one by one and set the new I2C address */
Dev=&devLeft;
HAL_GPIO_WritePin(GPIOH, SHUT1_Pin, GPIO_PIN_SET);
HAL_Delay(5);
Dev->comms_speed_khz = 400;
Dev->I2cHandle = &hi2c1;
Dev->comms_type = 1;
Dev->I2cDevAddr=0x52; /* default ToF sensor I2C address*/
newI2C = Dev->I2cDevAddr + 2;
VL53L1_SetDeviceAddress(Dev, newI2C);
Dev->I2cDevAddr=newI2C;
/* Device Initialization and setting */
VL53L1_WaitDeviceBooted(Dev);
VL53L1_DataInit(Dev);
VL53L1_StaticInit(Dev);
VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, 40000);
VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, 50);
memset ( &DetectionConfig,0,sizeof(DetectionConfig));
DetectionConfig.DetectionMode = VL53L1_DETECTION_DISTANCE_ONLY;
DetectionConfig.IntrNoTarget = 0;
DetectionConfig.Distance.CrossMode = VL53L1_THRESHOLD_IN_WINDOW;
DetectionConfig.Distance.Low = 20;
DetectionConfig.Distance.High = 300;
VL53L1_SetThresholdConfig ( Dev, &DetectionConfig);
VL53L1_UserRoi_t roiConfig;
roiConfig.TopLeftX= 0;
roiConfig.TopLeftY= 15;
roiConfig.BotRightX= 15;
roiConfig.BotRightY= 0;
VL53L1_SetUserROI(Dev, &roiConfig);
VL53L1_ClearInterruptAndStartMeasurement(Dev);
}
