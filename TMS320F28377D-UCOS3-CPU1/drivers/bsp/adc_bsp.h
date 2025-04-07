/**
 * @file adc_bsp.h
 * @brief 
 * @author hjroyal 
 * @version 1.0
 * @date 2024-08-30
 * 
 * @copyright Copyright (c) 2024  XXXX
 * 
 * @par Modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2024-08-30     <td>1.0       <td>hjroyal     <td>
 * </table>
 */

#ifndef ADC_BSP_H
#define ADC_BSP_H

/*
*********************************************************************************************************
*                                              DATA DEFINITION
*                                        (Data interfaces, constants, etc.)
*********************************************************************************************************
*/

//常量定义


#define EX_ADC_RESOLUTION       12
// 12 for 12-bit conversion resolution, which supports (ADC_MODE_SINGLE_ENDED)
// Sample on single pin (VREFLO is the low reference)
// Or 16 for 16-bit conversion resolution, which supports (ADC_MODE_DIFFERENTIAL)
// Sample on pair of pins (difference between pins is converted, subject to
// common mode voltage requirements; see the device data manual)

#define cACQPS           19

#define TEMP_SENSOR_ACQPS  139  //temperature sensor needs at least 700ns
                                //acquisition time

                                
//结构体定义


//联合体定义


//对外部文件开放全局变量声明




/*
*********************************************************************************************************
*                                              FUNCTION PROTOTYPES
*                                        (Golbal function prototypes.   )
*********************************************************************************************************
*/

//对外部文件开放函数声明
void InitADC_BSP(u32 adcBase);
void InitADCSOC(void);
interrupt void adcA1ISR(void);
#endif   /* ADC_BSP_H */


