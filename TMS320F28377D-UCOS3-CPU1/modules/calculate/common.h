/**
 * @file common.h
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

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HOST_SIM
/// Simulation platform head file
#else
/// Real platform head file
#endif

#include "stdint.h"   //TIC2000 types
#include "hw_types.h" //TIC2000 types
#include "math.h"
#include "types.h"
/*
*********************************************************************************************************
*                                              DATA API
*                                        (外部开放的数据接口)
*********************************************************************************************************
*/

#define FALSE 0
#define TRUE  1

/*重定义一些基本数据类型*/
typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef int                s32;
typedef unsigned int       u32;
typedef float              f32;
typedef double             f64;
typedef signed long long   s64;
typedef unsigned long long u64;

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;  /* Unsigned  8 bit quantity                             */
typedef signed char    INT8S;  /* Signed    8 bit quantity                             */
typedef unsigned short INT16U; /* Unsigned 16 bit quantity                             */
typedef signed short   INT16S; /* Signed   16 bit quantity                             */
typedef unsigned long  INT32U; /* Unsigned 32 bit quantity                             */
typedef signed long    INT32S; /* Signed   32 bit quantity                             */
typedef float          FP32;   /* Single precision floating point                      */
typedef long double    FP64;   /* Double precision floating point                      */


typedef int                Int;
typedef unsigned           Uns;
typedef char               Char;
typedef char              *String;
typedef void              *Ptr;
typedef unsigned short     Bool;
typedef unsigned char      Uint8;
typedef long               Int32;
typedef int                Int16;
typedef char               Int8;


/*
*********************************************************************************************************
*                                              FUNCTION API
*                                         (外部开放的函数接口)
*********************************************************************************************************
*/

#if defined(HOST_SIM)
// Simulation platform function
#endif

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
