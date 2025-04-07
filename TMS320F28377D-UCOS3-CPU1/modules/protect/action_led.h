/**
 * @file action_led.h
 * @brief 
 * @author hjroyal 
 * @version 1.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2024  XXXX
 * 
 * @par Modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2025-03-01     <td>1.0       <td>hjroyal     <td>update
 * </table>
 */

#ifndef ACTION_LED_H
#define ACTION_LED_H


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

#endif
