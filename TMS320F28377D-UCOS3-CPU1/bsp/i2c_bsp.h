/**
 * @file i2c_bsp.h
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

#ifndef I2C_BSP_H
#define I2C_BSP_H

#include "include.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Data interfaces, constants, etc.
//
//*****************************************************************************

//常量定义


                                
//结构体定义


//联合体定义


//对外部文件开放全局变量声明

extern u16 rtc_sec, rtc_min, rtc_hour, rtc_day, rtc_month, rtc_year;


//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//对外部文件开放函数声明
void initI2cRTC(void);
void getRtcTime(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif   /* I2C_BSP_H */


