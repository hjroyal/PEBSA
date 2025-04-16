/**
 * @file gpio_bsp.h
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

#ifndef GPIO_BSP_H
#define GPIO_BSP_H


/*
*********************************************************************************************************
*                                              DATA DEFINITION
*                                        (Data interfaces, constants, etc.)
*********************************************************************************************************
*/
//宏定义--------------------------------------------------------------------------------------------------

//自定义LED灯的GPIO口
#define LED1_ON GpioDataRegs.GPASET.bit.GPIO0 = 1
#define LED1_OFF GpioDataRegs.GPACLEAR.bit.GPIO0 = 1
#define LED1_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1

#define LED2_ON GpioDataRegs.GPASET.bit.GPIO1 = 1
#define LED2_OFF GpioDataRegs.GPACLEAR.bit.GPIO1 = 1
#define LED2_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1

#define LED3_ON GpioDataRegs.GPASET.bit.GPIO2 = 1
#define LED3_OFF GpioDataRegs.GPACLEAR.bit.GPIO2 = 1
#define LED3_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1

#define LED4_ON GpioDataRegs.GPASET.bit.GPIO3 = 1
#define LED4_OFF GpioDataRegs.GPACLEAR.bit.GPIO3 = 1
#define LED4_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1

#define LED5_ON GpioDataRegs.GPASET.bit.GPIO4 = 1
#define LED5_OFF GpioDataRegs.GPACLEAR.bit.GPIO4 = 1
#define LED5_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1

#define LED6_ON GpioDataRegs.GPASET.bit.GPIO5 = 1
#define LED6_OFF GpioDataRegs.GPACLEAR.bit.GPIO5 = 1
#define LED6_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO5 = 1

#define BLINK(LED) {\
    EALLOW;             \
    LED##_TOGGLE;       \
    EDIS;               \
}



//自定义步进机的四相GPIO口
#define StepA_ON   GpioDataRegs.GPASET.bit.GPIO16=1; 
#define StepA_OFF  GpioDataRegs.GPACLEAR.bit.GPIO16=1;
#define StepB_ON   GpioDataRegs.GPASET.bit.GPIO17=1;
#define StepB_OFF  GpioDataRegs.GPACLEAR.bit.GPIO17=1;
#define StepC_ON   GpioDataRegs.GPASET.bit.GPIO18=1;
#define StepC_OFF  GpioDataRegs.GPACLEAR.bit.GPIO18=1;
#define StepD_ON   GpioDataRegs.GPASET.bit.GPIO19=1;
#define StepD_OFF  GpioDataRegs.GPACLEAR.bit.GPIO19=1;

//beep
#define BEEP_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO30 = 1

//segment display
#define   SBIT3     GpioDataRegs.GPBDAT.bit.GPIO58
#define   SBIT2     GpioDataRegs.GPBDAT.bit.GPIO59
#define   SBIT1     GpioDataRegs.GPBDAT.bit.GPIO60
#define   SBIT0     GpioDataRegs.GPBDAT.bit.GPIO61



//relay
#define RELAY_TOGGLE GpioDataRegs.GPCTOGGLE.bit.GPIO67 = 1

//常量定义-----------------------------------------------------------------------------------------------


//结构体定义---------------------------------------------------------------------------------------------


//联合体定义---------------------------------------------------------------------------------------------


//对外部文件开放全局变量声明-------------------------------------------------------------------------------



/*
*********************************************************************************************************
*                                              FUNCTION PROTOTYPES
*                                        (Golbal function prototypes.   )
*********************************************************************************************************
*/
extern void InitGPIO_BSP(void);


#endif   /* GPIO_BSP_H */


