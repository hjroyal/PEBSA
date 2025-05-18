/**
 * @file f28377d_init.h
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

#ifndef F28377D_INIT_H
#define F28377D_INIT_H

#include "include.h"
/*
*********************************************************************************************************
*                                              DATA DEFINITION
*                                        (Data interfaces, constants, etc.)
*********************************************************************************************************
*/
// 宏定义
#define Enter_Critical() asm(" setc INTM")
#define Exit_Critical()  asm(" clrc INTM")

// 常量定义

// 结构体定义

// 联合体定义

// 对外部文件开放全局变量声明

/*
*********************************************************************************************************
*                                              FUNCTION PROTOTYPES
*                                        (Golbal function prototypes.   )
*********************************************************************************************************
*/

extern void initF28377d(void);
void        runtimeStart(void);
void        runtimeStop(void);

#endif /* F28377D_INIT_H */

/************************ (C) COPYRIGHT 2024 XXXX *****END OF FILE****/
