/**
 * @file pwm_bsp.h
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

#ifndef PWM_BSP_H
#define PWM_BSP_H

#include "include.h"

/*
*********************************************************************************************************
*                                              DATA DEFINITION
*                                        (Data interfaces, constants, etc.)
*********************************************************************************************************
*/

//常量定义


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
void initEpwmBSP(void);

void osTimePWMTrigger(void);

extern interrupt void epwm1_isr(void);

#endif   /* PWM_BSP_H */


